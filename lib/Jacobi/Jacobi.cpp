#include "Jacobi.h"

Jacobi::Jacobi()
{}


void Jacobi::solve
(
    const lduMatrix& system,
    nodeScalarField& phi
)
{
    const int Nx = phi.Nx();
    const int Ny = phi.Ny();

    for (int i = 1; i < Nx - 1; i++)
    {
        for (int j = 1; j < Ny - 1; j++)
        {
            int center = i + j * Nx;
            int centerInternal = phi.internalConvert(center);

            double coeffDiag = system.diag()[centerInternal];
            double solution = 0.0;

            // Upper stencil
            for (int f = system.ownerStart()[centerInternal];
                 f < system.ownerStart()[centerInternal + 1];
                 f++)
            {
                double phiNeighbor = phi.getValue(system.upperAddr()[f]);
                double coeffUpper = system.upper()[f];

                solution += coeffUpper * phiNeighbor / coeffDiag;
            }

            // Lower stencil
            for (int f = system.losortStart()[centerInternal];
                 f < system.losortStart()[centerInternal + 1];
                 f++)
            {
                double phiOwner = phi.getValue(system.lowerAddr()[f]);
                double coeffLower = system.lower()[f];

                solution += coeffLower * phiOwner / coeffDiag;
            }
        }
    }
}


bool Jacobi::convergence
(
    const lduMatrix& system,
    nodeScalarField& phi
)
{
    
}