#include "utility/volField.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
volField<Type>::volField(const orthoSquareMesh& mesh)
:
    Field<Type>(mesh, mesh.nCells())
{
    std::vector<std::string>& patchN = mesh.patchNames();
}