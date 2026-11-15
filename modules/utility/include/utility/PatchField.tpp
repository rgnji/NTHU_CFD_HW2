#include "utility/PatchField.h"
#include "utility/Patch.h"
#include "utility/volField.h"
#include <stdexcept>


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
PatchField<Type>::PatchField
(
    const volField<Type>& vol,
    const Patch& patch,
    const Dictionary dict
)
:
    Field<Type>(patch.faceCells().size()),
    vol_(vol),
    patch_(patch)
{}


template<class Type>
PatchField<Type>::PatchField
(
    const PatchField& oldField,
    const volField<Type>& internal
)
:
    Field<Type>(oldField.field()),
    vol_(internal),
    patch_(oldField.patch_)
{}


// * * * * * * * * * * * * * * * *  Selector * * * * * * * * * * * * * * * * //

template<class Type>
std::unique_ptr<PatchField<Type>> PatchField<Type>::New
(
    const volField<Type>& vol,
    const Patch& patch,
    const Dictionary dict
)
{
    auto it = table().find(dict.get<std::string>("type"));

    if (it == table().end())
        throw std::runtime_error("Unknown PatchField type: " + name);

    return it->second(vol, patch, dict);
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
Field<Type> PatchField<Type>::patchInternalField() const
{
    Field<Type> field(this->size());
    auto& owner = patch_.get()->faceCells();

    for 
    (int k=0; k<owner.size(); k++)
    {
        field.set(k, vol_.get(owner[k]));
    }

    return field;
}


template<class Type>
Field<Type> PatchField<Type>::snGrad() const
{
    Field<Type> field(this->size());
    auto& deltaCoeffs = this->patch().deltaCoeffs();
    auto& faceCells = patch_.get()->faceCells();

    for (int k=0; k<this->size(); k++)
    {

        field.set
        (
            k, 
            deltaCoeffs[k] * (this->get(k) - this->vol().get(faceCells[k]))
        );
    }

    return field;
}


template<class Type>
const volField<Type>& PatchField<Type>::vol() const
{
    return vol_;
}


template<class Type>
const Patch& PatchField<Type>::patch() const
{
    return patch_.get();
}