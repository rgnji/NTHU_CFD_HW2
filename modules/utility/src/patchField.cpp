#include "utility/patchField.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
patchField<Type>::patchField
(
    const orthoSquareMesh& mesh, 
    const std::vector<Type>& internal,
    std::string name
)
:
    Field<Type>(mesh),
    patch<Type>(mesh, internal, name)
{}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
const Type patchField<Type>::patchInternalField(int faceIndex) const
{
    int owner = this->mesh_.owner().at(faceIndex);
    return this->internal_.at(owner);
}