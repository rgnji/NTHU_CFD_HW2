#ifndef upwind_H
#define upwind_H

#include "convectionScheme/convectionScheme.h"
#include "utility/surfaceField.h"


namespace convectionSchemeType
{

template<class Type>
class upwind
:
    public convectionScheme<Type>
{
public:

    // Member functions

        // Weights
        virtual surfaceField<double> weights
        (
            const surfaceField<double>& phi
        ) const override;
};

}


#include "convectionScheme/upwind.tpp"


#endif