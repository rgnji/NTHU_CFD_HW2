#include "convectionScheme/center.h"


template<class Type>
surfaceField<double> convectionSchemeType::center<Type>::weights
(
    const surfaceField<double>& phi
) const
{
    return surfaceField<double>(phi.mesh(), 0.5);
}