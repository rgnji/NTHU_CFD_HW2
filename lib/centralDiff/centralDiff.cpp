#include "centralDiff.h"
#include <utility>
#include <vector>
#include <cmath>

diffusionScheme::centralDiff::centralDiff(bool ghost)
:
    ghost_(ghost)
{}


lduMatrix diffusionScheme::centralDiff::secondScheme
(
    const nodeScalarField& phi,
    const nodeScalarField& X,
    const nodeScalarField& Y
)
{
    const int Nx = phi.Nx();
    const int Ny = phi.Ny();

    lduMatrix A(phi);
    std::vector<lduMatrix::pairType> pairs;

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

    A.buildLduAddressing(phi, pairs);

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
                A.buildUpper(neighborInternal, 1/pow(hX, 2));
                // a_ji, j = P, i = W, i < j
                A.buildDiag(neighborInternal, -1/pow(hX, 2));
                A.buildLower(ownerInternal, 1/pow(hX, 2));
                break;

            case lduMatrix::direction::N:
                // a_ij, i = P, j = N, i < j
                A.buildDiag(ownerInternal, -1/pow(hY, 2));
                A.buildUpper(neighborInternal, 1/pow(hY, 2));
                // a_ji, j = P, i = S, i < j
                A.buildDiag(neighborInternal, -1/pow(hY, 2));
                A.buildLower(ownerInternal, 1/pow(hY, 2));  
                break;
        }
    }

    return A;
}


lduMatrix diffusionScheme::centralDiff::fourthScheme
(
    const nodeScalarField& phi,
    const nodeScalarField& X,
    const nodeScalarField& Y
)
{

}