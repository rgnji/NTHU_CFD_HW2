#include "utility/volField.h"
#include "utility/orthoSquareMesh.h"
#include "utility/Dictionary.h"
#include "utility/PatchField.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
volField<Type>::volField
(
    const std::string& name,
    const orthoSquareMesh& mesh, 
    const Dictionary& dict
)
:
    Field<Type>(mesh.nCells()),
    name_(name),
    mesh_(mesh)
{
    std::vector<std::string> ps{"left", "right", "bottom", "top"};
    for (std::string key : ps)
    {
        Dictionary subDict = dict.subDict(key);
        boundary_[key] = 
            PatchField<Type>::New(*this, mesh.patches(key), dict.subDict(key));
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
const orthoSquareMesh& volField<Type>::mesh() const
{
    return mesh_;
}
