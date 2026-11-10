#ifndef patchField_H
#define patchField_H

#include "utility/Field.h"
#include "utility/orthoSquareMesh.h"
#include <string>

template<class Type>
class patchField
:
    public Field<Type>
{
    // Private data

        // Patch name
        std::string name_;

        // start face index
        int startFace_;

        // face number
        int nFaces_;


public:

    // Constructor

        // Construct from mesh with name
        patchField(const orthoSquareMesh& mesh, std::string name);
};


#endif