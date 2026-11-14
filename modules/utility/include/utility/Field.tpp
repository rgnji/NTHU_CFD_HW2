#include "utility/Field.h"


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
Field<Type>::Field(int N)
:
    N_(N),
    field_(N)
{}


template<class Type>
Field<Type>::Field(const std::vector<Type>& field)
:
    N_(field.size()),
    field_(field)
{}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
int Field<Type>::total() const
{
    return N_;
}


template<class Type>
void Field<Type>::set(int N, Type value)
{
    field_[N] = value;
}


template<class Type>
Type Field<Type>::get(int N) const
{
    return field_[N];
}


template<class Type>
void Field<Type>::resize(int size)
{
    N_ = size;
    field_.resize(size);
}