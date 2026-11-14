#include "utility/orthoSquareMesh.h"
#include "utility/Patch.h"
#include "utility/Dictionary.h"
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

                buildFaces
                (
                    j * Nx_ + (i + 1),
                    (j + 1) * Nx_ + (i + 1),
                    {1.0, 0.0},
                    dy_
                );

                buildFaceCenter(owner+1, owner+1+Nx_);
            }

            // N
            if (j + 1 < cellNumY)
            {
                owner_.push_back(owner);
                neighbor_.push_back(owner + cellNumX);

                buildFaces
                (
                    (j + 1) * Nx_ + i,
                    (j + 1) * Nx_ + (i + 1),
                    {0.0, 1.0},
                    dx_
                );

                buildFaceCenter(owner+Nx_, owner+Nx_+1);
            }

            // owner index = point index at bottom left of the cell
            buildCellCenter(owner, owner+1, owner+Nx_);
        }
    }

    buildPatch();
}


void orthoSquareMesh::buildPatch()
{
    int cellNumX = Nx_ - 1;
    int cellNumY = Ny_ - 1;

    // left patch
    patches_["left"] = std::make_unique<Patch>
        (*this, "left", int(faces_.size()), cellNumY);

    for (int j=0; j<cellNumY; j++)
    {
        owner_.push_back(j * cellNumX);

        buildFaces
        (
            j * Nx_,
            (j + 1) * Nx_,
            {-1.0, 0.0},
            dy_
        );

        buildFaceCenter(j * cellNumX, j * cellNumX + Nx_);
    }

    // right patch
    patches_["right"] = std::make_unique<Patch>
        (*this, std::string("right"), int(faces_.size()), cellNumY);

    for (int j=0; j<cellNumY; j++)
    {
        owner_.push_back((j + 1) * cellNumX - 1);

        buildFaces
        (
            (j + 1) * Nx_ - 1,
            (j + 2) * Nx_ - 1,
            {1.0, 0.0},
            dy_
        );

        buildFaceCenter((j + 1) * cellNumX, (j + 1) * cellNumX + Nx_);
    }

    // bottom patch
    patches_["bottom"] = std::make_unique<Patch>
        (*this, std::string("bottom"), int(faces_.size()), cellNumX);

    for (int i=0; i<cellNumX; i++)
    {
        owner_.push_back(i);

        buildFaces
        (
            i,
            i + 1,
            {0.0, -1.0},
            dx_
        );

        buildFaceCenter(i, i+1);
    }
    
    // top patch
    patches_["top"] = std::make_unique<Patch>
        (*this, std::string("top"), int(faces_.size()), cellNumX);

    for (int i=0; i<cellNumX; i++)
    {
        owner_.push_back((cellNumY - 1) * cellNumX + i);

        buildFaces
        (
            (Ny_ - 1) * Nx_ + i,
            (Ny_ - 1) * Nx_ + i + 1,
            {0.0, 1.0},
            dx_
        );

        buildFaceCenter
        (
            (cellNumY - 1) * cellNumX + i + Nx_,
            (cellNumY - 1) * cellNumX + i + Nx_ + 1
        );
    }
}


void orthoSquareMesh::buildFaces
(
    int pointOwner,
    int pointNbr,
    Vector p,
    double len
)
{
    std::vector<int> face{pointOwner, pointNbr};
    faces_.push_back(face);
    Nf_.push_back(p);
    Sf_.push_back(len);
}


void orthoSquareMesh::buildCellCenter(int ownerPoint, int nbrPointX, int nbrPointY)
{
    cellCenter_.push_back
    (
        {
            0.5*(points_[ownerPoint].x + points_[nbrPointX].x),
            0.5*(points_[ownerPoint].y + points_[nbrPointY].y)
        }
    );
}


void orthoSquareMesh::buildFaceCenter(int ownerPoint, int nbrPoint)
{
    faceCenter_.push_back
    (
        {
            0.5*(points_[ownerPoint].x + points_[nbrPoint].x),
            0.5*(points_[ownerPoint].y + points_[nbrPoint].y)
        }
    );
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

const std::vector<std::vector<int>>& orthoSquareMesh::faces() const
{
    return faces_;
}


const std::vector<Vector>& orthoSquareMesh::Nf() const
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


const std::unique_ptr<Patch>& orthoSquareMesh::patches
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


const std::vector<Vector>& 
orthoSquareMesh::cellCenter() const
{
    return cellCenter_;
}


const std::vector<Vector>&
orthoSquareMesh::faceCenter() const
{
    return faceCenter_;
}