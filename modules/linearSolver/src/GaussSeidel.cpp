#include "linearSolver/GaussSeidel.h"
#include "utility/lduMatrix.h"


namespace
{
    linearSolver::Register<GaussSeidel> addGaussSeidel("GaussSeidel");
}


void GaussSeidel::sweep
(
    const lduMatrix& A,
    std::vector<double>& x,
    const std::vector<double>& b
) const
{
    const auto& diag  = A.diag();
    const auto& upper = A.upper();
    const auto& lower = A.lower();
    const auto& u     = A.upperAddr();
    const auto& os    = A.ownerStart();

    const int nCells = static_cast<int>(x.size());

    // Right-hand side minus lower triangle
    std::vector<double> bPrime(b);

    for (int p=0; p<nCells; p++)
    {
        double xP = bPrime[p];

        for (int f=os[p]; f<os[p + 1]; f++)
            xP -= upper[f] * x[u[f]];

        xP /= diag[p];

        for (int f=os[p]; f<os[p + 1]; f++)
            bPrime[u[f]] -= lower[f] * xP;

        x[p] = xP;
    }
}


GaussSeidel::SolverPerformance GaussSeidel::solve
(
    const lduMatrix& A,
    std::vector<double>& x,
    const std::vector<double>& b
) const
{
    SolverPerformance perf;
    perf.initialResidual = A.residualNorm(x, b);
    perf.finalResidual   = perf.initialResidual;

    while (perf.nIterations < maxIter_ && !converged(perf))
    {
        sweep(A, x, b);
        perf.finalResidual = A.residualNorm(x, b);
        ++perf.nIterations;
    }

    perf.converged = converged(perf);
    return perf;
}