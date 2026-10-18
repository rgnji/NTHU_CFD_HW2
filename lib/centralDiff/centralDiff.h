#ifndef centralDiff_H
#define centralDiff_H

#include "lduMatrix/lduMatrix.h"
#include "scalarField/scalarField.h"

namespace diffusionScheme
{

class centralDiff
{
    // Ghost node flag
    bool ghost_;


public:

    // Constructor

        // Construct from order of accuracy
        centralDiff(bool ghost=true);

    
    // Member functions

        // Second-order accuracy scheme
        lduMatrix secondScheme
        (
            const nodeScalarField& phi,
            const nodeScalarField& X,
            const nodeScalarField& Y
        );

        // Fourth-order accuracy scheme
        /*
        lduMatrix fourthScheme
        (
            const nodeScalarField& phi,
            const nodeScalarField& X,
            const nodeScalarField& Y
        );*/
};

};

#endif 