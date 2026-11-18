#include "convectionScheme/convectionScheme.h"

#include "convectionScheme/upwind.h"
#include "convectionScheme/QUICK.h"
#include "convectionScheme/center.h"


namespace
{
    convectionScheme<double>::Register<convectionSchemeType::upwind<double>>
        addUpwindScalar("upwind");

    convectionScheme<double>::Register<convectionSchemeType::QUICK<double>>
        addQUICKScalar("QUICK");

    convectionScheme<double>::Register<convectionSchemeType::center<double>>
        addCenterScalar("center");
}
