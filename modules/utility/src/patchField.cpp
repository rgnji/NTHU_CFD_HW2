#include "utility/PatchField.h"
#include <stdexcept>


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
PatchField<Type>::PatchField
(
    const volField<Type>& vol,
    const Patch& patch,
    const SubDict& dict
)
:
    Field<Type>(),
    vol_(vol),
    patch_(patch)
{}


// * * * * * * * * * * * * * * * *  Selector * * * * * * * * * * * * * * * * //

template<class Type>
static std::unique_ptr<PatchField<Type>> PatchField<Type>::New
(
    const orthoSquareMesh& mesh,
    const std::vector<Type>& internal,
    const std::string name
)
{
    auto it = table().find(name);

    if (it == table().end())
        throw std::runtime_error("Unknown PatchField type: " + name);

    return it->second(mesh, internal, name);
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
const Type PatchField<Type>::patchInternalField(int faceIndex) const
{
    
}