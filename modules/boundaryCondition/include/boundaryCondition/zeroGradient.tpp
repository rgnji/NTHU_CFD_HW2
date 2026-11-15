#include "boundaryCondition/zeroGradient.h"
#include "utility/pTraits.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
zeroGradient<Type>::zeroGradient
(
    const volField<Type>& vol,
    const Patch& patch,
    const Dictionary dict
)
:
    PatchField<Type>(vol, patch, dict)
{
    auto& faceCells = patch.faceCells();

    for (int k=0; k<this->size(); k++)
    {
        this->set(k, vol.get(faceCells[k]));
    }
}


template<class Type>
zeroGradient<Type>::zeroGradient
(
    const zeroGradient& oldField,
    const volField<Type>& internal
)
:
    PatchField<Type>(oldField, internal),
{}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
std::unique_ptr<zeroGradient<Type>> zeroGradient<Type>::clone
(
    const volField<Type>& newField
) const
{
    return zeroGradient(*this, newField);
}


template<class Type>
void zeroGradient<Type>::evaluate()
{
    auto& faceCells = this->patch().faceCells();

    for (int k=0; k<this->size(); k++)
    {
        this->set(k, this->vol().get(faceCells[k]));
    }
}


template<class Type>
Field<Type> zeroGradient<Type>::snGrad() const
{
    return Field<Type>(this->size(), pTraits<Type>::one * 0);
}


template<class Type>
Field<Type> zeroGradient<Type>::valueInternalCoeffs() const
{
    return Field<Type>(this->size(), pTraits<Type>::one);
}


template<class Type>
Field<Type> zeroGradient<Type>::valueBoundaryCoeffs() const
{
    return Field<Type>(this->size(), pTraits<Type>::one * 0);
}


template<class Type>
Field<Type> zeroGradient<Type>::gradientInternalCoeffs() const
{
    return Field<Type>(this->size(), pTraits<Type>::one * 0);
}


template<class Type>
Field<Type> zeroGradient<Type>::gradientBoundaryCoeffs() const
{
    return Field<Type>(this->size(), pTraits<Type>::one * 0);
}