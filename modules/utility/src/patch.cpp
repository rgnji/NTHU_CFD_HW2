#include "utility/patch.h"
#include <cmath>


// * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * * //

template<class Type>
patch<Type>::patch
(
    const orthoSquareMesh& mesh,
    const std::vector<Type>& internal,
    std::string name
)
:
    name_(name),
    startFace_(mesh.patchInfo(name).startFace),
    nFaces_(mesh.patchInfo(name).nFaces),
    internal_(internal),
    owner_(std::span<int>(mesh.owner().data() + startFace_, nFaces_)),
    deltaCoeff_(nFaces_)
{
    buildDeltaCoeff(mesh);
}


// * * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * //

template<class Type>
void patch<Type>::buildDeltaCoeff(const orthoSquareMesh& mesh)
{
    const std::vector<orthoSquareMesh::coordinate>& 
        cellCenter = mesh.cellCenter();
    const std::vector<orthoSquareMesh::coordinate>& 
        faceCenter = mesh.faceCenter();
    
    for (int k=0; k<nFaces_; k++)
    {
        int face = startFace_ + k;
        int cell = mesh.owner().at(face);

        double xDis = abs(cellCenter[cell].x - faceCenter[face].x);
        double yDis = abs(cellCenter[cell].y - faceCenter[face].y);

        deltaCoeff_.push_back(1.0 / pow(xDis * xDis + yDis * yDis, 0.5));
    }
}


// * * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * //

template<class Type>
const std::span<const int> patch<Type>::faceCells() const
{
    return owner_;
}


template<class Type>
const std::vector<double>& patch<Type>::deltaCoeff() const
{
    return deltaCoeff_;
}