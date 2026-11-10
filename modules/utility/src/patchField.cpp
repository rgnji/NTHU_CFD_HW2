#include "utility/patchField.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
patchField<Type>::patchField(const orthoSquareMesh& mesh, std::string name)
:
    Field<Type>(mesh),
    name_(name),
    startFace_(mesh.patchInfo(name).startFace),
    nFaces_(mesh.patchInfo(name).nFaces)
{
    this->resize(nFaces_);
}