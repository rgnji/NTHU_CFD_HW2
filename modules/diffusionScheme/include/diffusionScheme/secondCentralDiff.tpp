#include "diffusionScheme/secondCentralDiff.h"
#include "field/volField.h"
#include "field/surfaceField.h"
#include "field/PatchField.h"
#include <vector>


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
fvMatrix<Type> diffusionScheme::secondCentralDiff<Type>::interpolate
(
    const surfaceField<Type>& phi,
    volField<Type>& field
)
{
    fvMatrix<Type> m(field);
    int nInternalFaces = field.mesh().nInternalFaces();

    for (int k=0; k<nInternalFaces; k++)
    {
        auto& deltaCoeffs = field.mesh().deltaCoeffs();
        int owner = field.mesh().owner().at(k);
        int nbr = field.mesh().neighbor().at(k);

        m.buildDiag(owner, phi.get(k) * deltaCoeffs.at(k));
        m.buildDiag(nbr, phi.get(k) * deltaCoeffs.at(k));
        m.buildLower(k, - phi.get(k) * deltaCoeffs.at(k));
        m.buildUpper(k, - phi.get(k) * deltaCoeffs.at(k));
    }

    for (int p=0; p<field.boundary().size(); p++)
    {
        const PatchField<Type>& pF = *(field.boundary().at(p));
        const Patch& patch = pF.patch();

        for (int k=0; k<patch.nFaces(); k++)
        {
            m.buildInternalCoeffs
            (
                p,
                k,
                - phi.get(patch.startFace() + k)
                * pF.gradientInternalCoeffs().get(k)
            );
            m.buildBoundaryCoeffs
            (
                p,
                k,
                phi.get(patch.startFace() + k)
              * pF.gradientBoundaryCoeffs().get(k)
            );
        }
    }

    return m;
}