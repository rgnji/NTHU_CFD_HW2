#ifndef Jacobi_H
#define Jacobi_H

#include <vector>
#include <utility>
#include "nodeScalarField/nodeScalarField.h"
#include "lduMatrix/lduMatrix.h"

class Jacobi
{
    // Private data member

        // Convergence criterion
        const double epsilon_ = 1e-6;

        // Maximum iteration
        const int maxIt = 10000;

        // New solution
        nodeScalarField newSolution_;

        // Initial residual
        double r0_;

        // Residual history
        std::vector<double> rHistory_;


    // Private member functions

        // Precede one iteration
        double solve
        (
            const lduMatrix& system,
            nodeScalarField& phi
        );


public:

    // Constructor

        Jacobi(const nodeScalarField& phi);


    // Member functions

        // Determine whether the solution is converged
        std::pair<bool, int> iterate
        (
            const lduMatrix& system,
            nodeScalarField& phi
        );

        // Return residual history
        const std::vector<double>& rHistory() const;
};


#endif