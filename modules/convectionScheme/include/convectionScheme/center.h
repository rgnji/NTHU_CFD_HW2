#ifndef center_H
#define center_H

#include "convectionScheme/convectionScheme.h"
#include "field/surfaceField.h"


namespace convectionSchemeType
{

template<class Type>
class center
:
    public convectionScheme<Type>
{
public:

    // Member Functions

        // Weights
        virtual surfaceField<double> weights
        (
            const surfaceField<double>& phi
        ) const override;
};
}


#include "convectionScheme/center.tpp"


#endif