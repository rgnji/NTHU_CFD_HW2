#include "nodeScalarField.h"

nodeScalarField::nodeScalarField(int Nx, int Ny)
:
    scalarField(Nx * Ny),
    Nx_(Nx),
    Ny_(Ny)
{
    for (int j=1; j<Ny-1; j++)
    {
        for (int i=1; i<Nx-1; i++)
        {
            internalNodeList_.push_back(i + j * Nx);
        }
    }
}


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


int nodeScalarField::face() const
{
    return (Ny_ - 2)*(Nx_ - 2)*2 + (Nx_ + Ny_);
}


const std::vector<int>& nodeScalarField::internalNodeList() const
{
    return internalNodeList_;
}