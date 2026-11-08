#include "utility/orthoSquareMesh.h"
#include <algorithm>
#include <iterator>

// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

orthoSquareMesh::orthoSquareMesh(int Nx, int Ny, double dx, double dy)
:
    Nx_(Nx),
    Ny_(Ny),
    dx_(dx),
    dy_(dy),
    points_(Nx_*Ny_),
    nCells_((Nx_-1)*(Ny_-1))
{
    build();
}


// * * * * * * * * * * * * * Private Member Functions * * * * * * * * * * * *//

void orthoSquareMesh::build()
{
    // Build points
    for (int j=0; j<Nx_; j++)
    {
        for (int i=0; i<Ny_; i++)
        {
            int index = i * Nx_ + j;
            points_[index] = {i*dx_, j*dy_};
        }
    }

    // Build owner, neighbor and faces
    int cellNumX = Nx_ - 1;
    int cellNumY = Ny_ - 1;

    for (int j=0; j<cellNumY; j++)
    {
        for (int i=0; i<cellNumX; i++)
        {
            int owner = j * cellNumX + i;

            // E
            if (i + 1 < cellNumX)
            {
                owner_.push_back(owner);
                neighbor_.push_back(owner + 1);

                int pointOwner = j * Nx_ + (i + 1);
                int pointNeighbor = (j + 1) * Nx_ + (i + 1);
                std::vector<int> face{pointOwner, pointNeighbor};

                faces_.push_back(face);
                Nf_.push_back({1.0, 0.0});
                Sf_.push_back(dy_);
            }

            // N
            if (j + 1 < cellNumY)
            {
                owner_.push_back(owner);
                neighbor_.push_back(owner + cellNumX);

                int pointOwner = (j + 1) * Nx_ + i;
                int pointNeighbor = (j + 1) * Nx_ + (i + 1);
                std::vector<int> face{pointOwner, pointNeighbor};

                faces_.push_back(face);
                Nf_.push_back({0.0, 1.0});
                Sf_.push_back(dx_);
            }
        }
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

const std::vector<std::vector<int>>& orthoSquareMesh::faces() const
{
    return faces_;
}


const std::vector<orthoSquareMesh::coordinate>& orthoSquareMesh::Nf() const
{
    return Nf_;
}


const std::vector<double>& orthoSquareMesh::Sf() const
{
    return Sf_;
}


const std::vector<int>& orthoSquareMesh::owner() const
{
    return owner_;
}


const std::vector<int>& orthoSquareMesh::neighbor() const
{
    return neighbor_;
}


int orthoSquareMesh::nCells() const
{
    return nCells_;
}