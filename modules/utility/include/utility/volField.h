#ifndef volField_H
#define volField_H

#include "utility/Field.h"
#include "utility/patchField.h"
#include <string>
#include <map>


template<class Type>
class volField
:
    public Field<Type>
{
    // Private data

        struct Patch
        {
            std::string name_;
            patchField patch_;
            
        }

        // Field name
        std::string name_;

        // Patch fields
        std::map<std::string, Patch> volPatch_;
public:

    // Constructor

        // Construct from mesh
        volField(const orthoSquareMesh& mesh);
};


#endif