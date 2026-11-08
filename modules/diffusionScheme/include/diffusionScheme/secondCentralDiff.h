#ifndef secondCentralDiff_H
#define secondCentralDiff_H

#include "utility/lduMatrix.h"
#include "utility/scalarField.h"

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
        lduMatrix interpolate
        (
            const orthoSquareMesh& phi,
            const orthoSquareMesh& X,
            const orthoSquareMesh& Y
        );
};

};

#endif 