#include "field/volField.h"
#include "utility/orthoSquareMesh.h"
#include "utility/Dictionary.h"
#include "field/PatchField.h"
#include <fstream>
#include <format>
#include <type_traits>


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
volField<Type>::volField
(
    const std::string& name,
    const orthoSquareMesh& mesh, 
    const Dictionary& dict
)
:
    Field<Type>(mesh.nCells(), dict.get<double>("initialValue")),
    name_(name),
    mesh_(mesh)
{
    const auto& patches = mesh.patches();
    const Dictionary& patchDict = dict.subDict("patches");
    
    for (int k=0; k<patches.size(); k++)
    {
        std::string name = patches.at(k).name();

        boundary_.push_back
        (
            PatchField<Type>::New
                (*this, patches.at(k), patchDict.subDict(name))
        );
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
void volField<Type>::correctBoundaryConditions()
{
    for (int p=0; p<boundary_.size(); p++)
    {
        boundary_[p]->evaluate();
    }
}


template<class Type>
void volField<Type>::write(std::string path, bool includeBoundary) const
{
    static_assert(std::is_same_v<Type, double>, 
        "write(path, includeBoundary) is only for scalar field.");

    std::ofstream file(path);
    file << "x,y," << name_ << "\n";

    const auto& C = mesh_.C();
    for (int k=0; k<C.size(); k++)
    {
        file << std::format("{:.8e},{:.8e},{:.8e}\n", C[k].x, C[k].y, this->get(k));
    }

    if (includeBoundary)
    {
        const auto& Cf = mesh_.Cf();
        for (int p=0; p<boundary_.size(); p++)
        {
            const Patch& patch = boundary_[p]->patch();
            for (int k=0; k<patch.nFaces(); k++)
            {
                const int& f = patch.startFace() + k;
                file << std::format
                (
                    "{:.8e},{:.8e},{:.8e}\n", 
                    Cf[f].x, Cf[f].y, boundary_[p]->get(f)
                );
            }
        }
    }
}