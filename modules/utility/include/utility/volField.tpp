#include "utility/volField.h"
#include "utility/orthoSquareMesh.h"
#include "utility/Dictionary.h"
#include "utility/PatchField.h"
#include <fstream>
#include <format>


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
    auto& patches = mesh.patches();
    
    for (int k=0; k<patches.size(); k++)
    {
        std::string name = patches[k].name();
        Dictionary subDict = dict.subDict(name);

        boundary_.push_back
        (
            PatchField<Type>::New(*this, mesh.patches[k], dict.subDict(name))
        );
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
void volField<Type>::write(std::string path) const
{
    std::ofstream file(path);
    
    // Store the field as 1D arrary
    for (int k=0; k<mesh_.nCells()-1; k++)
    {
        file << std::format("{:.6f}", get(k)) << ",";
    }
    file << std::format("{:.6f}", get(mesh_.nCells()-1));
    file.close();
}