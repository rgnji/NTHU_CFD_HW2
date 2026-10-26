#ifndef upwind_H
#define upwind_H

#include "utility/lduMatrix.h"
#include "utility/nodeScalarField.h"

namespace convectionScheme
{

class upwind
{
public:

    // Member functions

        // Upwind scheme
        void construct
        (
            lduMatrix& diffusion,
            const nodeScalarField& X,
            const nodeScalarField& Y,
            const nodeScalarField& phi
        );
};

};

#endif