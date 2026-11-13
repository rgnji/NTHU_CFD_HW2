#ifndef orthoSquareMesh_H
#define orthoSquareMesh_H

#include "utility/Vector.h"
#include "utility/Patch.h"
#include <vector>
#include <string>
#include <map>
#include <memory>


class orthoSquareMesh
{
    // Private data

        // Mesh sizes (include boundary)
        int Nx_, Ny_;
        double dx_, dy_;
        
        // Total cells
        int nCells_;

        // Points
        std::vector<Vector> points_;

        // Faces(2 points form a face in 2D meshes)
        std::vector<std::vector<int>> faces_;

        // Face normal
        std::vector<Vector> Nf_;

        // Face size
        std::vector<double> Sf_;

        // Cell of smaller index in a pair
        std::vector<int> owner_;

        // Cell of larger index in a pair
        std::vector<int> neighbor_;

        // Cell centers
        std::vector<Vector> cellCenter_;

        // Face centers
        std::vector<Vector> faceCenter_;

        // Patch information
        std::map<std::string, std::unique_ptr<Patch>> patches_;

    // Private Member Functions

        // Arrange internal mesh information
        void build();

        void buildFaces(int pointOwner, int pointNbr, Vector p, double len);

        void buildCellCenter(int ownerPoint, int nbrPointX, int nbrPointY);

        void buildFaceCenter(int ownerPoint, int nbrPoint);

        // Arrange patch information
        void buildPatch();


public:

    // Constructor

        // Construct from element number and mesh sizes
        orthoSquareMesh(int Nx, int Ny, double dx, double dy);


    // Member functions

        // Return reference of faces
        const std::vector<std::vector<int>>& faces() const;

        // Return reference of Nf
        const std::vector<Vector>& Nf() const;

        // Return reference of Sf
        const std::vector<double>& Sf() const;

        // Return reference of owner
        const std::vector<int>& owner() const;

        // Return reference of neighbor
        const std::vector<int>& neighbor() const;

        // Return specific patch information
        const std::unique_ptr<Patch>& patches(std::string name) const;

        // Return total cells
        int nCells() const;

        // Return cell centers
        const std::vector<Vector>& cellCenter() const;

        // Return face centers
        const std::vector<Vector>& faceCenter() const;
};


#endif