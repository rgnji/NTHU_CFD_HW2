#include "utility/orthoSquareMesh.h"
#include "utility/Patch.h"
#include "utility/Dictionary.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cmath>

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
    buildDeltaCoeffs();
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
            buildCell(owner, owner+1, owner+Nx_);
        }
    }

    buildPatch();
}


void orthoSquareMesh::buildPatch()
{
    int cellNumX = Nx_ - 1;
    int cellNumY = Ny_ - 1;

    // left patch
    patches_.push_back
    (
        Patch(*this, "left", int(faces_.size()), cellNumY)
    );

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
    patches_.push_back
    (
        Patch(*this, "right", int(faces_.size()), cellNumY)
    );

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
    patches_.push_back
    (
        Patch(*this, "bottom", int(faces_.size()), cellNumX)
    );

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
    patches_.push_back
    (
        Patch(*this, "top", int(faces_.size()), cellNumX)
    );

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
    int ownerPoint,
    int nbrPoint,
    Vector p,
    double len
)
{
    std::vector<int> face{ownerPoint, nbrPoint};
    faces_.push_back(face);
    Sf_.push_back(p);
    magSf_.push_back(len);
}


void orthoSquareMesh::buildCell(int ownerPoint, int nbrPointX, int nbrPointY)
{
    C_.push_back
    (
        {
            0.5*(points_[ownerPoint].x + points_[nbrPointX].x),
            0.5*(points_[ownerPoint].y + points_[nbrPointY].y)
        }
    );

    V_.push_back
    (
        std::abs(points_[ownerPoint].x - points_[nbrPointX].x)
      * std::abs(points_[ownerPoint].y - points_[nbrPointY].y)
    );
}


void orthoSquareMesh::buildFaceCenter(int ownerPoint, int nbrPoint)
{
    Cf_.push_back
    (
        {
            0.5*(points_[ownerPoint].x + points_[nbrPoint].x),
            0.5*(points_[ownerPoint].y + points_[nbrPoint].y)
        }
    );
}


void orthoSquareMesh::buildDeltaCoeffs()
{
    for (int k=0; k<neighbor_.size(); k++)
    {
        int owner = owner_[k];
        int nbr = neighbor_[k];
        Vector ownerC = C_[owner];
        Vector nbrC = C_[nbr];

        deltaCoeffs_.push_back
        (
            1.0
          / pow
            (
                (ownerC.x - nbrC.x) * (ownerC.x - nbrC.x)
              + (ownerC.y - nbrC.y) * (ownerC.y - nbrC.y),
                0.5
            )
        );
    }

    for (int k=0; k<patches_.size(); k++)
    {
        deltaCoeffs_.insert
        (
            deltaCoeffs_.end(),
            patches_[k].deltaCoeffs().begin(),
            patches_[k].deltaCoeffs().end()
        );
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

const Patch& orthoSquareMesh::findPatch(std::string name) const
{
    for (int k=0; k<patches_.size(); k++)
        if (name == patches_[k].name())
            return patches_[k];

    std::cout << 
    "Not a patch name. (At orthoSquareMesh::patchInfo(std::string)): "
    << name << std::endl;

    std::abort();
}