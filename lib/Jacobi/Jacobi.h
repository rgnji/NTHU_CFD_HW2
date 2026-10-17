#ifndef Jacobi_H
#define Jacobi_H

#include "nodeScalarField.h"
#include "lduMatrix.h"

class Jacobi
{
    double epsilon_;


public:

    // Constructor

        Jacobi();


    // Member functions

        // Precede one iteration
        void solve
        (
            const lduMatrix& system,
            nodeScalarField& phi
        );

        // Determine whether the solution is converged
        bool convergence
        (
            const lduMatrix& system,
            nodeScalarField& phi
        );
};


#endif