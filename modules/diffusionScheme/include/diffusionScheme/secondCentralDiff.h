#ifndef secondCentralDiff_H
#define secondCentralDiff_H

#include "utilHeader/fvMatrix.h"


template<class Type>
class volField;

template<class Type>
class surfaceField;


namespace diffusionScheme
{

template<class Type>
class secondCentralDiff
{
public:

    // Member functions

        fvMatrix<Type> interpolate
        (
            const surfaceField<Type>& phi,
            volField<Type>& field
        );
};

}


#include "diffusionScheme/secondCentralDiff.tpp"


#endif 