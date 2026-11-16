#ifndef GaussSeidel_H
#define GaussSeidel_H

#include "linearSolver/linearSolver.h"
#include <vector>


class lduMatrix;


class GaussSeidel
:
    public linearSolver
{
    // Private Member Functions

        void sweep
        (
            const lduMatrix& A,
            std::vector<double>& x,
            const std::vector<double>& b
        ) const;


public:

    // Constructor
    using linearSolver::linearSolver;


    // Member Functions

        SolverPerformance solve
        (
            const lduMatrix& A,
            std::vector<double>& x,
            const std::vector<double>& b
        ) const override;
};


#endif