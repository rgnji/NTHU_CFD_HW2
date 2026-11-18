#include "utility/Patch.h"
#include "utility/orthoSquareMesh.h"
#include <cmath>


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

Patch::Patch
(
    const orthoSquareMesh& mesh,
    const std::string name,
    const int startFace,
    const int nFaces
)
:
    mesh_(mesh),
    name_(name),
    startFace_(startFace),
    nFaces_(nFaces),
    deltaCoeff_(nFaces_)
{
    buildDeltaCoeff(mesh);
}


// * * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * //

void Patch::buildDeltaCoeff(const orthoSquareMesh& mesh)
{
    const std::vector<Vector>& cellCenter = mesh.C();
    const std::vector<Vector>& faceCenter = mesh.Cf();
    
    for (int k=0; k<nFaces_; k++)
    {
        int face = startFace_ + k;
        int cell = mesh.owner().at(face);

        double xDis = abs(cellCenter[cell].x - faceCenter[face].x);
        double yDis = abs(cellCenter[cell].y - faceCenter[face].y);

        deltaCoeff_[k] = 1.0 / pow(xDis * xDis + yDis * yDis, 0.5);
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

std::span<const int> Patch::faceCells() const
{
    const std::vector<int>& owner = mesh_.owner();

    return std::span<const int>(owner.data() + startFace_, nFaces_);
}