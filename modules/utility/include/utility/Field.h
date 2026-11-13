#ifndef Field_H
#define Field_H

#include <vector>
#include "utility/orthoSquareMesh.h"

template<class Type>
class Field
{
    // Element number
    int N_;

    // 1D field object
    std::vector<Type> field_;


public:

    // Constructor

        // Construct from element number
        Field(int N);


    // Member functions

        // Return total element number
        int total() const;

        // Setter
        void set(int N, const Type value);

        // Getter
        Type get(int N) const;

        // Determine field size
        void resize(int size);
};


#endif