#ifndef Field_H
#define Field_H

#include <vector>


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

        // Construct from field
        Field(const std::vector<Type>& field);


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


#include "utility/Field.tpp"


#endif