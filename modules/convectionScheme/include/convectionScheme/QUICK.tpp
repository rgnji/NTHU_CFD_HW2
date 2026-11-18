#include "convectionScheme/QUICK.h"
#include "utilHeader/fvMatrix.h"
#include "field/volField.h"
#include "field/PatchField.h"
#include "utility/orthoSquareMesh.h"
#include "utility/Patch.h"


// * * * * * * * * * * * * * Private Member Functions * * * * * * * * * * * //

template<class Type>
Field<Vector> convectionSchemeType::QUICK<Type>::gaussGradient
(
    const volField<Type>& vf
) const
{
    const orthoSquareMesh& mesh = vf.mesh();
    const auto& owner = mesh.owner();
    const auto& nbr   = mesh.neighbor();
    const auto& Sf    = mesh.Sf();
    const auto& V     = mesh.V();

    Field<Vector> grad(mesh.nCells(), Vector{0.0, 0.0});

    // Internal faces: linear interpolation (uniform mesh, weight 0.5)
    for (int f=0; f<mesh.nInternalFaces(); f++)
    {
        const double phif = 0.5 * (vf.get(owner[f]) + vf.get(nbr[f]));
        const Vector flux = Sf[f] * phif;

        grad.set(owner[f], grad.get(owner[f]) + flux);
        grad.set(nbr[f],   grad.get(nbr[f])   - flux);  // Sf points out of owner
    }

    // Boundary faces: face values from the boundary conditions
    for (int p=0; p<static_cast<int>(mesh.patches().size()); p++)
    {
        const PatchField<Type>& pf = vf.boundary(p);
        const int start = pf.patch().startFace();

        for (int k=0; k<pf.size(); k++)
        {
            const int f = start + k;
            grad.set(owner[f], grad.get(owner[f]) + Sf[f] * pf.get(k));
        }
    }

    for (int c=0; c<mesh.nCells(); c++)
        grad.set(c, grad.get(c) * (1.0 / V[c]));

    return grad;
}


template<class Type>
surfaceField<Type> convectionSchemeType::QUICK<Type>::faceValue
(
    const volField<Type>& vf,
    const surfaceField<double>& phi
) const
{
    const orthoSquareMesh& mesh = vf.mesh();
    const auto& owner = mesh.owner();
    const auto& nbr   = mesh.neighbor();
    const auto& C     = mesh.C();

    const Field<Vector> grad = gaussGradient(vf);
    surfaceField<Type> face(mesh);

    // Internal faces
    for (int f=0; f<mesh.nInternalFaces(); f++)
    {
        const bool fromOwner = phi.get(f) >= 0.0;
        const int c = fromOwner ? owner[f] : nbr[f];    // upstream cell
        const int d = fromOwner ? nbr[f]   : owner[f];  // downstream cell

        const double phiC = vf.get(c);
        const double phiD = vf.get(d);
        const Vector dCD  = C[d] - C[c];
        const double phiU = phiD - 2.0 * (dCD & grad.get(c));

        face.set(f, 0.75 * phiC + 0.375 * phiD - 0.125 * phiU);
    }

    // Boundary faces: determined by the boundary conditions
    for (int p=0; p<static_cast<int>(mesh.patches().size()); p++)
    {
        const PatchField<Type>& pf = vf.boundary(p);
        const int start = pf.patch().startFace();

        for (int k=0; k<pf.size(); k++)
            face.set(start + k, pf.get(k));
    }

    return face;
}


template<class Type>
void convectionSchemeType::QUICK<Type>::addCorrection
(
    fvMatrix<Type>& eqn,
    const surfaceField<double>& phi
) const
{
    const volField<Type>& vf = eqn.psi();
    const orthoSquareMesh& mesh = vf.mesh();
    const auto& owner = mesh.owner();
    const auto& nbr   = mesh.neighbor();

    const surfaceField<Type> faceHO = faceValue(vf, phi);

    // Boundary faces need no correction: both schemes use the boundary value
    for (int f=0; f<mesh.nInternalFaces(); f++)
    {
        const double F = phi.get(f);
        const double faceUD = (F >= 0.0) ? vf.get(owner[f]) : vf.get(nbr[f]);
        const double corr = F * (faceHO.get(f) - faceUD);

        eqn.addSource(owner[f], -corr);   // extra outflow from owner
        eqn.addSource(nbr[f],    corr);   // extra inflow to neighbour
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
surfaceField<double> convectionSchemeType::QUICK<Type>::weights
(
    const surfaceField<double>& phi
) const
{
    surfaceField<double> w(phi.mesh());

    for (int f=0; f<phi.mesh().nFaces(); f++)
        w.set(f, (phi.get(f) >= 0.0) ? 1.0 : 0.0);

    return w;
}


template<class Type>
void convectionSchemeType::QUICK<Type>::correct
(
    fvMatrix<Type>& eqn,
    const surfaceField<double>& phi
) const
{
    addCorrection(eqn, phi);
}