#include "convectionScheme/upwind.h"


template<class Type>
surfaceField<double> convectionSchemeType::upwind<Type>::weights
(
    const surfaceField<double>& phi
) const
{
    surfaceField<double> w(phi.mesh());

    for (int f=0; f<phi.mesh().nFaces(); f++)
        w.set(f, (phi.get(f) >= 0.0) ? 1.0 : 0.0);

    return w;
}