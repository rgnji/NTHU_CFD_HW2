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

        // Field name
        std::string name_;

        // Patch field list
        std::vector<patchField<Type>> pFieldList_;

    // Private Member List

        // Build patch field list
        void buildPField();


public:

    // Constructor

        // Construct from mesh
        volField(const orthoSquareMesh& mesh);


    // Member Functions

        // Set initial internal field
        void setInternal();
};


#endif