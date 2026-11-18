#include "field/surfaceField.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
surfaceField<Type>::surfaceField(const orthoSquareMesh& mesh)
:
    Field<Type>(mesh.nFaces()),
    mesh_(mesh)
{}


template<class Type>
surfaceField<Type>::surfaceField
(
    const orthoSquareMesh& mesh,
    Type value
)
:
    Field<Type>(mesh.nFaces(), value),
    mesh_(mesh)
{}


template<class Type>
surfaceField<Type>::surfaceField
(
    const orthoSquareMesh& mesh,
    const std::vector<Type>& field
)
:
    Field<Type>(field),
    mesh_(mesh)
{}