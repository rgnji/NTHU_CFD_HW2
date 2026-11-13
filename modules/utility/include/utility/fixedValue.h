#ifndef fixedValue_H
#define fixedValue_H

#include "utility/PatchField.h"


template<class Type>
class fixedValue
:
    public PatchField<Type>
{
    // Private data

        // Boundary value
        Type value_;


public:

    // Constructor

        fixedValue
        (
            const orthoSquareMesh& mesh,
            const std::vector<Type>& internal,
            const std::string name
        );
};


#endif