#ifndef secondCentralDiff_H
#define secondCentralDiff_H

#include "utility/fvMatrix.h"


template<class Type>
class volField;

template<class Type>
class surfaceField;


namespace diffusionScheme
{

class secondCentralDiff
{
public:

    // Constructor

        // Construct from order of accuracy
        secondCentralDiff();

    
    // Member functions

        template<class Type>
        fvMatrix<Type> interpolate
        (
            const surfaceField<Type>& phi,
            const volField<Type>& field
        );
};

};

#endif 