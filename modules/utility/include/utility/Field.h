#ifndef Field_H
#define Field_H

#include "utility/pTraits.h"
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

        // Construct from size and initial value
        Field(int size, pTraits<Type> value);


    // Member functions

        // Return total element number
        int size() const;

        // Setter
        void set(int N, const Type value);

        // Getter
        Type get(int N) const;

        // Determine field size
        void resize(int size);

        // Return the whole field
        const std::vector<Type>& field() const
        {
            return field_;
        }

        // Allow editting
        std::vector<Type>& fieldList()
        {
            return field_;
        }
};


#include "utility/Field.tpp"


#endif