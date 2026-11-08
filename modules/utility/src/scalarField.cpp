#include "utility/scalarField.h"

scalarField::scalarField(int N)
:
    N_(N),
    field_(N, 0.0)
{}


int scalarField::total() const
{
    return N_;
}


void scalarField::setValue(int N, double value)
{
    field_[N] = value;
}


double scalarField::getValue(int N) const
{
    return field_[N];
}