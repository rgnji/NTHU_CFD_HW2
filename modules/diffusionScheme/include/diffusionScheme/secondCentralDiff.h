#ifndef secondCentralDiff_H
#define secondCentralDiff_H

#include "utility/lduMatrix/lduMatrix.h"
#include "utility/scalarField/scalarField.h"

namespace diffusionScheme
{

class secondCentralDiff
{
    // Ghost node flag
    bool ghost_;


public:

    // Constructor

        // Construct from order of accuracy
        secondCentralDiff(bool ghost=true);

    
    // Member functions

        // Second-order accuracy scheme
        lduMatrix secondScheme
        (
            const nodeScalarField& phi,
            const nodeScalarField& X,
            const nodeScalarField& Y
        );
};

};

#endif 