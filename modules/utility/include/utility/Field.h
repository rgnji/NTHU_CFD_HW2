#ifndef Field_H
#define Field_H

#include <vector>
#include "utility/orthoSquareMesh.h"

template<class Type>
class Field
{
    const orthoSquareMesh mesh_;

    // Element number
    int N_;

    // 1D field object
    std::vector<Type> field_;


public:

    // Constructor

        // Construct from element number
        Field(const orthoSquareMesh& mesh, int N);

        // Construct without field size
        Field(const orthoSquareMesh& mesh);


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