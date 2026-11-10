#include "utility/orthoSquareMesh.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstdlib>

// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

orthoSquareMesh::orthoSquareMesh(int Nx, int Ny, double dx, double dy)
:
    Nx_(Nx),
    Ny_(Ny),
    dx_(dx),
    dy_(dy),
    nCells_((Nx_-1)*(Ny_-1)),
    points_(Nx_*Ny_)
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

    buildPatch();
}


void orthoSquareMesh::buildPatch()
{
    int cellNumX = Nx_ - 1;
    int cellNumY = Ny_ - 1;

    // left patch
    patchNames_.push_back(std::string("left"));
    patches_["left"] = {std::string("left"), int(faces_.size()), cellNumY};
    for (int j=0; j<cellNumY; j++)
    {
        owner_.push_back(j * cellNumX);
        int pointOwner = j * Nx_;
        int pointNeighbor = (j + 1) * Nx_;
        std::vector<int> face{pointOwner, pointNeighbor};
        faces_.push_back(face);
        Nf_.push_back({-1.0, 0.0});
        Sf_.push_back(dy_);
    }

    // right patch
    patchNames_.push_back(std::string("right"));
    patches_["right"] = {std::string("right"), int(faces_.size()), cellNumY};
    for (int j=0; j<cellNumY; j++)
    {
        owner_.push_back((j + 1) * cellNumX - 1);
        int pointOwner = (j + 1) * Nx_ - 1;
        int pointNeighbor = (j + 2) * Nx_ - 1;
        std::vector<int> face{pointOwner, pointNeighbor};
        faces_.push_back(face);
        Nf_.push_back({1.0, 0.0});
        Sf_.push_back(dy_);
    }

    // bottom patch
    patchNames_.push_back(std::string("bottom"));
    patches_["bottom"] = {std::string("bottom"), int(faces_.size()), cellNumX};
    for (int i=0; i<cellNumX; i++)
    {
        owner_.push_back(i);
        int pointOwner = i;
        int pointNeighbor = i + 1;
        std::vector<int> face{pointOwner, pointNeighbor};
        faces_.push_back(face);
        Nf_.push_back({0.0, -1.0});
        Sf_.push_back(dx_);
    }
    
    // top patch
    patchNames_.push_back(std::string("top"));
    patches_["top"] = {std::string("top"), int(faces_.size()), cellNumX};
    for (int i=0; i<cellNumX; i++)
    {
        owner_.push_back((cellNumY - 1) * cellNumX + i);
        int pointOwner = (Ny_ - 1) * Nx_ + i;
        int pointNeighbor = (Ny_ - 1) * Nx_ + i + 1;
        std::vector<int> face{pointOwner, pointNeighbor};
        faces_.push_back(face);
        Nf_.push_back({0.0, 1.0});
        Sf_.push_back(dx_);
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


const std::vector<std::string>& orthoSquareMesh::patchNames() const
{
    return patchNames_;
}


const orthoSquareMesh::Patch& orthoSquareMesh::patchInfo
(
    std::string name
) const
{
    try
    {
        return patches_.at(name);
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << 
        "Not a patch name. (At orthoSquareMesh::patchInfo(std::string)): "
        << e.what() << "\n";

        std::abort();
    }
}


int orthoSquareMesh::nCells() const
{
    return nCells_;
}