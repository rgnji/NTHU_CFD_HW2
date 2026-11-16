#include "utility/fvMatrix.h"
#include "utility/orthoSquareMesh.h"
#include "utility/Field.h"
#include "utility/volField.h"
#include "utility/Dictionary.h"
#include "utility/pTraits.h"
#include "utility/Patch.h"
#include <memory>


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
fvMatrix<Type>::fvMatrix(volField<Type>& psi)
:
    lduMatrix(psi.mesh()),
    psi_(psi),
    source_(psi.mesh().nCells(), pTraits<Type>::one * 0)
{
    for (int p=0; p<psi.boundary().size(); p++)
    {
        auto patch = *(psi.boundary().at(p));
        Field<Type> zero(patch.patch().nFaces(), pTraits<Type>::one * 0);

        internalCoeffs_.push_back(zero);
        boundaryCoeffs_.push_back(zero);
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
void fvMatrix<Type>::buildInternalCoeffs(int patch, int face, Type coeff)
{
    internalCoeffs_.at(patch).set(face, coeff);
}


template<class Type>
void fvMatrix<Type>::buildBoundaryCoeffs(int patch, int face, Type coeff)
{
    boundaryCoeffs_.at(patch).set(face, coeff);
}


template<class Type>
linearSolver::SolverPerformance fvMatrix<Type>::solve
(
    const linearSolver& solver
)
{
    auto totalSource = source_;
    auto diagOriginal = this->diag();

    for (int p=0; p<internalCoeffs_.size(); p++)
    {
        const Patch& patch = psi_.mesh().patches().at(p);

        for (int f=0; f<internalCoeffs_[p].size(); f++)
        {
            int owner = patch.faceCells(f);

            auto s = totalSource.get(owner);
            totalSource.set(owner, s + boundaryCoeffs_.at(p).get(f));
            this->buildDiag(owner, internalCoeffs_.at(p).get(f));
        }
    }

    auto result = solver.solve(*this, psi_.fieldList(), totalSource);
    this->diagList() = diagOriginal;

    return result;
}


template<class Type>
linearSolver::SolverPerformance fvMatrix<Type>::solve
(
    const Dictionary& dict
)
{
    const std::unique_ptr<linearSolver> solver = linearSolver::New(dict);
    return solve(*solver);
}