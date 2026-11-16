#ifndef volField_H
#define volField_H

#include "utility/Field.h"
#include <string>
#include <map>
#include <memory>


class Dictionary;

class orthoSquareMesh;

template<class Type>
class PatchField;


template<class Type>
class volField
:
    public Field<Type>
{
    // Private data

        // Field name
        std::string name_;

        // Reference to mesh
        orthoSquareMesh& mesh_;

        // Patch field list
        std::vector<std::unique_ptr<PatchField<Type>>> boundary_;

    // Private Member List

        // Build patch field list
        void buildPField();


public:

    // Constructor

        // Construct from mesh
        volField
        (
            const std::string& name,
            const orthoSquareMesh& mesh, 
            const Dictionary& dict
        );


    // Member Functions

        // Return mesh reference
        const orthoSquareMesh& mesh() const
        {
            return mesh_;
        }

        // Return boundary patches
        const std::vector<std::unique_ptr<PatchField<Type>>>& boundary() const
        {
            return boundary_;
        }

        // Return boundary by patch index
        const PatchField<Type>& boundary(int patch) const
        {
            return *(boundary_.at(patch))
        }

        // Output flowfield
        void write(std::string path) const;
};


#include "utility/volField.tpp"


#endif