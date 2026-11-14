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
    Field<Type>(),
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
    Field<Type>(oldField.field_),
    vol_(internal),
    patch_(oldField.patch_)


// * * * * * * * * * * * * * * * *  Selector * * * * * * * * * * * * * * * * //

template<class Type>
std::unique_ptr<PatchField<Type>> PatchField<Type>::New
(
    const volField<Type>& vol,
    const Patch& patch,
    const Dictionary dict
)
{
    auto it = table().find(name);

    if (it == table().end())
        throw std::runtime_error("Unknown PatchField type: " + name);

    return it->second(vol, patch, dict);
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
const Type PatchField<Type>::patchInternalField(int faceIndex) const
{
    const int owner = vol_.mesh().owner(faceIndex);
    return vol_.get(owner);
}


