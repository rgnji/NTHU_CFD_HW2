#include "utility/surfaceField.h"
#include "utility/orthoSquareMesh.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
surfaceField<Type>::surfaceField(const orthoSquareMesh& mesh)
:
    Field<Type>(mesh.owner().size()),
    mesh_(mesh)
{}