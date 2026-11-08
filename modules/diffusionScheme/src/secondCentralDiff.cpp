#include "diffusionScheme/secondCentralDiff.h"
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>

diffusionScheme::secondCentralDiff::secondCentralDiff(bool ghost)
:
    ghost_(ghost)
{}


lduMatrix diffusionScheme::secondCentralDiff::interpolate
(
    const orthoSquareMesh& phi,
    const orthoSquareMesh& X,
    const orthoSquareMesh& Y
)
{
    const int Nx = phi.Nx();
    const int Ny = phi.Ny();

    lduMatrix A(phi);
    std::vector<lduMatrix::pairType> pairs;
    std::vector<lduMatrix::pairType> boundaryPairs;

    // internal pairs
    for (int i = 1; i < Nx-1; i++)
    {
        for (int j = 1; j < Ny-1; j++)
        {
            // Index of center node
            const int index = i + j * Nx;

            // E
            if (i + 1 < Nx - 1)
            {
                std::pair<int, int> pairE(index, index + 1);
                pairs.push_back({pairE, lduMatrix::direction::E});
            }

            // N
            if (j + 1 < Ny - 1)
            {
                std::pair<int, int> pairN(index, index + Nx);
                pairs.push_back({pairN, lduMatrix::direction::N});
            }
        }
    }

    // internal-boundary pairs
    for (int i = 1; i < Nx-1; i++)
    {
        int lowerInternalIndex = i + Nx;
        int lowerBoundaryIndex = i;
        int upperInternalIndex = i + (Ny - 2) * Nx;
        int upperBoundaryIndex = i + (Ny - 1) * Nx;

        std::pair<int, int> pairLower(lowerInternalIndex, lowerBoundaryIndex);
        boundaryPairs.push_back({pairLower, lduMatrix::direction::S});
        std::pair<int, int> pairUpper(upperInternalIndex, upperBoundaryIndex);
        boundaryPairs.push_back({pairUpper, lduMatrix::direction::N});
    }

    for (int j = 1; j < Ny-1; j++)
    {
        int leftInternalIndex = 1 + j * Nx;
        int leftBoundaryIndex = j * Nx;
        int rightInternalIndex = (Nx - 2) + j * Nx;
        int rightBoundaryIndex = (Nx - 1) + j * Nx;

        std::pair<int, int> pairLeft(leftInternalIndex, leftBoundaryIndex);
        boundaryPairs.push_back({pairLeft, lduMatrix::direction::W});
        std::pair<int, int> pairRight(rightInternalIndex, rightBoundaryIndex);
        boundaryPairs.push_back({pairRight, lduMatrix::direction::E});
    }

    // build addressing
    A.buildLduAddressing(phi, pairs);
    A.buildBoundaryAddressing(phi, boundaryPairs);

    // build coefficients
    for (int k = 0; k < A.lowerAddr().size(); k++)
    {
        int owner = A.lowerAddr()[k];
        int neighbor = A.upperAddr()[k];

        int ownerInternal = phi.internalConvert(owner);
        int neighborInternal = phi.internalConvert(neighbor);

        double hX = X.getValue(owner) - X.getValue(neighbor);
        double hY = Y.getValue(owner) - Y.getValue(neighbor);

        switch(A.pairDirection()[k])
        {
            case lduMatrix::direction::E:
                // a_ij, i = P, j = E, i < j
                A.buildDiag(ownerInternal, -1/pow(hX, 2));
                A.buildUpper(k, 1/pow(hX, 2));
                // a_ji, j = P, i = W, i < j
                A.buildDiag(neighborInternal, -1/pow(hX, 2));
                A.buildLower(k, 1/pow(hX, 2));
                break;

            case lduMatrix::direction::N:
                // a_ij, i = P, j = N, i < j
                A.buildDiag(ownerInternal, -1/pow(hY, 2));
                A.buildUpper(k, 1/pow(hY, 2));
                // a_ji, j = P, i = S, i < j
                A.buildDiag(neighborInternal, -1/pow(hY, 2));
                A.buildLower(k, 1/pow(hY, 2));  
                break;

            default:
                std::cout << "Wrong direction in lduMatrix.pairDirection()!" << std::endl;
                break;
        }
    }

    // Account for boundary contribution
    for (int k = 0; k < A.internalAddr().size(); k++)
    {
        int inter = A.internalAddr()[k];
        int bound = A.boundaryAddr()[k];

        int interConvert = phi.internalConvert(inter);

        double hX = X.getValue(inter) - X.getValue(bound);
        double hY = Y.getValue(inter) - Y.getValue(bound);

        double boundValue = phi.getValue(bound);

        switch(A.boundaryDirection()[k])
        {
            case lduMatrix::direction::E:
                A.buildDiag(interConvert, -1/pow(hX, 2));
                A.buildB(interConvert, -1/pow(hX, 2)*boundValue);
                break;

            case lduMatrix::direction::W:
                A.buildDiag(interConvert, -1/pow(hX, 2));
                A.buildB(interConvert, -1/pow(hX, 2)*boundValue);
                break;

            case lduMatrix::direction::N:
                A.buildDiag(interConvert, -1/pow(hY, 2));
                A.buildB(interConvert, -1/pow(hY, 2)*boundValue);
                break;

            case lduMatrix::direction::S:
                A.buildDiag(interConvert, -1/pow(hY, 2));
                A.buildB(interConvert, -1/pow(hY, 2)*boundValue);
                break;

            default:
                std::cout << "Wrogn direction in lduMatrix.boundaryDirection()!" << std::endl;
                break;
        }
    }

    return A;
}

