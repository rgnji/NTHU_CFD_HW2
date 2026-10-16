#include "centralDiff.h"
#include <utility>
#include <vector>

diffusionScheme::centralDiff::centralDiff(bool ghost)
:
    ghost_(ghost)
{}


lduMatrix diffusionScheme::centralDiff::secondScheme
(
    nodeScalarField phi
)
{
    const int Nx = phi.Nx();
    const int Ny = phi.Ny();

    lduMatrix A(phi);
    std::vector<std::pair<int, int>> couplings;

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
                couplings.push_back(pairE);
            }

            // N
            if (j + 1 < Ny - 1)
            {
                std::pair<int, int> pairN(index, index + Nx);
                couplings.push_back(pairN);
            }
        }
    }

    A.buildLduAddressing(phi, couplings);

    return A;
}


lduMatrix diffusionScheme::centralDiff::fourthScheme
(
    nodeScalarField phi
)
{

}