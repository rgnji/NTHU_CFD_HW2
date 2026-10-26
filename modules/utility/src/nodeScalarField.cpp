#include "nodeScalarField.h"
#include <algorithm>
#include <iterator>

nodeScalarField::nodeScalarField(int Nx, int Ny)
:
    scalarField(Nx * Ny),
    Nx_(Nx),
    Ny_(Ny)
{}


int nodeScalarField::Nx() const
{
    return Nx_;
}


int nodeScalarField::Ny() const
{
    return Ny_;
}


int nodeScalarField::node() const
{
    return (Ny_ - 2) * (Nx_ - 2);
}


int nodeScalarField::internalConvert(int p) const
{
    int i = p % Nx_;
    int j = p / Nx_;

    return (i - 1) + (j - 1) * (Nx_ - 2);
}


int nodeScalarField::globalIndex(int P) const
{
    int i = P % (Nx_ - 2) + 1;
    int j = P / (Nx_ - 2) + 1;

    return i + j * Nx_;
}