#include "utility/Field.h"


template<class Type>
Field<Type>::Field(const orthoSquareMesh& mesh, int N)
:
    mesh_(mesh),
    N_(N),
    field_(N)
{}


template<class Type>
Field<Type>::Field(const orthoSquareMesh& mesh)
:
    mesh_(mesh)
{}


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