#include "convectionScheme/convectionScheme.h"
#include "utility/Dictionary.h"
#include "utilHeader/fvMatrix.h"
#include "utility/orthoSquareMesh.h"
#include "utility/Patch.h"


template<class Type>
std::unique_ptr<convectionScheme<Type>> convectionScheme<Type>::New
(
    const Dictionary& dict
)
{
    std::string name = dict.get<std::string>("convectionScheme");
    auto it = table().find(name);

    if (it == table().end())
        throw std::runtime_error("Unknown convection scheme: " + name);

    return it->second();
}


template<class Type>
void convectionScheme<Type>::add
(
    fvMatrix<Type>& eqn,
    const surfaceField<double>& phi,
    const surfaceField<double>& weights
)
{
    const orthoSquareMesh& mesh = phi.mesh();
    const auto& owner = mesh.owner();
    const auto& nbr = mesh.neighbor();

    auto& diag = eqn.diag();
    auto& lower = eqn.lower();
    auto& upper = eqn.upper();

    for (int f=0; f<mesh.nInternalFaces(); f++)
    {
        const double W  = weights.get(f) * phi.get(f);
        const double Wr = (1.0 - weights.get(f)) * phi.get(f);

        eqn.buildDiag(owner.at(f), W);
        eqn.buildUpper(f, Wr);
        eqn.buildDiag(nbr.at(f), -Wr);
        eqn.buildLower(f, -W);
    }

    const auto& psi = eqn.psi();
    for (int i=0; i<static_cast<int>(mesh.patches().size()); i++)
    {
        const Patch& p = mesh.patches().at(i);
        const Field<Type> alpha = psi.boundary(i).valueInternalCoeffs();
        const Field<Type> beta  = psi.boundary(i).valueBoundaryCoeffs();

        for (int k=0; k<p.nFaces(); k++)
        {
            const double phiB = phi.get(p.startFace() + k);

            eqn.buildInternalCoeffs(i, k, phiB * alpha.get(k));
            eqn.buildBoundaryCoeffs(i, k, - phiB * beta.get(k));
        }
    }
}


template<class Type>
void convectionScheme<Type>::add
(
    fvMatrix<Type>& eqn,
    const surfaceField<double>& phi
)
{
    this->add(eqn, phi, this->weights(phi));
}