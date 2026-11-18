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


orthoSquareMesh::orthoSquareMesh(const Dictionary& dict)
:
    Nx_(dict.get<std::vector<int>>("node").at(0)),
    Ny_(dict.get<std::vector<int>>("node").at(1)),
    dx_(dict.get<Vector>("size").x),
    dy_(dict.get<Vector>("size").y),
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
            int index = j * Nx_ + i;
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

                buildFaceCenter
                (
                    j * Nx_ + i + 1,
                    (j + 1) * Nx_ + i + 1
                );
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

                buildFaceCenter
                (
                    (j + 1) * Nx_ + i,
                    (j + 1) * Nx_ + i + 1
                );
            }

            // owner index = point index at bottom left of the cell
            buildCell
            (
                j * Nx_ + i,
                j * Nx_ + i + 1,
                (j + 1) * Nx_ + i
            );
        }
    }

    buildPatch();
}


void orthoSquareMesh::buildPatch()
{
    int cellNumX = Nx_ - 1;
    int cellNumY = Ny_ - 1;

    // left patch
    int leftStart = faces_.size();
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

        buildFaceCenter(j * Nx_, (j + 1) * Nx_);
    }

    // right patch
    int rightStart = faces_.size();
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

        buildFaceCenter((j + 1) * Nx_ - 1, (j + 2) * Nx_ - 1);
    }

    // bottom patch
    int bottomStart = faces_.size();
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
    int topStart = faces_.size();
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
            (Ny_ - 1) * Nx_ + i,
            (Ny_ - 1) * Nx_ + i + 1
        );
    }

    patches_.push_back
    (
        Patch(*this, "left", leftStart, cellNumY)
    );
    patches_.push_back
    (
        Patch(*this, "right", rightStart, cellNumY)
    );
    patches_.push_back
    (
        Patch(*this, "bottom", bottomStart, cellNumX)
    );
    patches_.push_back
    (
        Patch(*this, "top", topStart, cellNumX)
    );
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
    Sf_.push_back(p * len);
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