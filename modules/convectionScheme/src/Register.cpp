#include "convectionScheme/convectionScheme.h"

#include "convectionScheme/upwind.h"


namespace
{
    convectionScheme<double>::Register<convectionSchemeType::upwind<double>>
        addUpwindScalar("upwind");
}
