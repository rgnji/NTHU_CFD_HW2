#include "Jacobi.h"
#include <cmath>

Jacobi::Jacobi(const nodeScalarField& phi)
:
    newSolution_(phi.Nx(), phi.Ny())
{}


double Jacobi::solve
(
    const lduMatrix& system,
    nodeScalarField& phi
)
{
    const int Nx = phi.Nx();
    const int Ny = phi.Ny();
    double error = 0.0;

    newSolution_ = phi;

    for (int i = 1; i < Nx - 1; i++)
    {
        for (int j = 1; j < Ny - 1; j++)
        {
            int center = i + j * Nx;
            int centerInternal = phi.internalConvert(center);

            double coeffDiag = system.diag()[centerInternal];
            double solution = 0.0;
            double newValue = 0.0;

            // Upper stencil
            for (int f = system.ownerStart()[centerInternal];
                 f < system.ownerStart()[centerInternal + 1];
                 f++)
            {
                double phiNeighbor = phi.getValue(system.upperAddr()[f]);
                double coeffUpper = system.upper()[f];

                solution += coeffUpper * phiNeighbor;
            }

            // Lower stencil
            for (int f = system.losortStart()[centerInternal];
                 f < system.losortStart()[centerInternal + 1];
                 f++)
            {
                int ff = system.losortAddr()[f];
                double phiOwner = phi.getValue(system.lowerAddr()[ff]);
                double coeffLower = system.lower()[ff];

                solution += coeffLower * phiOwner;
            }

            // right-hand side
            solution = system.b()[centerInternal] - solution;
            newValue = solution / coeffDiag;

            // update solution
            newSolution_.setValue(center, newValue);
        }
    }

    phi = newSolution_;

    return system.residualNorm(phi);
}


std::pair<bool, int> Jacobi::iterate
(
    const lduMatrix& system,
    nodeScalarField& phi
)
{
    r0_ = system.residualNorm(phi);

    for (int it = 0; it < maxIt; it++)
    {
        double r = solve(system, phi);
        rHistory_.push_back(r);

        if (std::abs(r / r0_) < epsilon_)
        {
            return std::pair<bool, int>(true, it + 1);
        }
    }

    return std::pair<bool, int>(false, maxIt);
}


const std::vector<double>& Jacobi::rHistory() const
{
    return rHistory_;
}