#include "boundaryCondition/fixedValue.h"
#include "utility/Dictionary.h"
#include "utility/pTraits.h"
#include "utility/Patch.h"
#include "utility/volField.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
fixedValue<Type>::fixedValue
(
    const volField<Type>& vol,
    const Patch& patch,
    const Dictionary dict
)
:
    PatchField<Type>(vol, patch, dict)
{
    for (int k=0; k<this->size(); k++)
    {
        this->set(k, dict.get<Type>("value"));
    }
}


template<class Type>
fixedValue<Type>::fixedValue
(
    const fixedValue& oldField,
    const volField<Type>& internal
)
:
    PatchField<Type>(oldField, internal),
{}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
std::unique_ptr<fixedValue<Type>> fixedValue<Type>::clone
(
    const volField<Type>& newField
) const
{
    return fixedValue(*this, newField);
}


template<class Type>
Field<Type> fixedValue<Type>::valueInternalCoeffs() const
{
    return Field<Type>(this->size(), pTraits<Type>::one * 0);
}


template<class Type>
Field<Type> fixedValue<Type>::valueBoundaryCoeffs() const
{
    return *this;
}


template<class Type>
Field<Type> fixedValue<Type>::gradientInternalCoeffs() const
{
    Field<Type> field(this->size());
    auto& deltaCoeffs = this->patch().deltaCoeffs();

    for (int k=0; k<this->size(); k++)
    {
        field.set(k, - deltaCoeffs[k] * pTraits<Type>::one);
    }

    return field;
}


template<class Type>
Field<Type> fixedValue<Type>::gradientBoundaryCoeffs() const
{
    Field<Type> field(this->size());
    auto& deltaCoeffs = this->patch().deltaCoeffs();

    for (int k=0; k<this->size(); k++)
    {
        field.set(k, deltaCoeffs[k] * this->get(k));
    }

    return field;
}