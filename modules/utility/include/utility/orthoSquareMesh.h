#ifndef orthoSquareMesh_H
#define orthoSquareMesh_H

#include <vector>
#include <string>

class orthoSquareMesh
{
public:

    // Coordinates of points
    struct coordinate
    {
        double x,y;
    };

    // Patch information container
    struct patch
    {
        std::string name;
        int startFace;
        int nFaces;
    };

   
private:

    // Private data

        // Mesh sizes (include boundary)
        int Nx_, Ny_;
        double dx_, dy_;
        
        // Total cells
        int nCells_;

        // Points
        std::vector<coordinate> points_;

        // Faces(2 points form a face in 2D meshes)
        std::vector<std::vector<int>> faces_;

        // Face normal
        std::vector<coordinate> Nf_;

        // Face size
        std::vector<double> Sf_;

        // Cell of smaller index in a pair
        std::vector<int> owner_;

        // Cell of larger index in a pair
        std::vector<int> neighbor_;

        // Patch information
        std::vector<patch> patches_;

    // Private Member Functions

        // Calculate mesh information
        void build();


public:

    // Constructor

        // Construct from element number and mesh sizes
        orthoSquareMesh(int Nx, int Ny, double dx, double dy);


    // Member functions

        // Return reference of faces
        const std::vector<std::vector<int>>& faces() const;

        // Return reference of Nf
        const std::vector<coordinate>& Nf() const;

        // Return reference of Sf
        const std::vector<double>& Sf() const;

        // Return reference of owner
        const std::vector<int>& owner() const;

        // Return reference of neighbor
        const std::vector<int>& neighbor() const;

        // Return total cells
        int nCells() const;
};


#endif