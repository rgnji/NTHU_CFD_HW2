#ifndef surfaceField_H
#define surfaceField_H

#include "field/Field.h"


class orthoSquareMesh;


template<class Type>
class surfaceField
:
    public Field<Type>
{
    // Private Data

        // Mesh reference
        const orthoSquareMesh& mesh_;


public:

    // Constructor

        // Construct from mesh
        surfaceField(const orthoSquareMesh& mesh);


    // Member Functions

        // Return mesh object
        const orthoSquareMesh& mesh() const
        {
            return mesh_;
        }
};


#include "field/surfaceField.tpp"


#endif