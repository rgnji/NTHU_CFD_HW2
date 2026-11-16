#ifndef orthoSquareMesh_H
#define orthoSquareMesh_H

#include "utility/Vector.h"
#include <vector>
#include <string>
#include <map>
#include <memory>


class Dictionary;

class Patch;


class orthoSquareMesh
{
    // Private Data

        // Basic Data

            // Mesh sizes (nodes)
            int Nx_, Ny_;
            double dx_, dy_;
            
            // Total cells
            int nCells_;

            // Points
            std::vector<Vector> points_;

            // Faces(2 points form a face in 2D meshes)
            std::vector<std::vector<int>> faces_;

            // Cell of smaller index in a pair
            std::vector<int> owner_;

            // Cell of larger index in a pair
            std::vector<int> neighbor_;

            // Patch information
            std::vector<Patch> patches_;


        // Derived Data

            // Cell centers
            std::vector<Vector> C_;

            // Cell volume
            std::vector<double> V_;

            // Face centers
            std::vector<Vector> Cf_;

            // Face normal
            std::vector<Vector> Sf_;

            // Face size
            std::vector<double> magSf_;

            // Reciprocal of cell center distance (include boundary)
            std::vector<double> deltaCoeffs_;

            // Interpolation weights
            // std::vector<double> weights_;

            // Vector between cell centers for non-orthogonal correction
            // std::vector<Vector> delta_;


    // Private Member Functions

        // Arrange internal mesh information
        void build();

        void buildFaces(int pointOwner, int pointNbr, Vector p, double len);

        void buildCell(int ownerPoint, int nbrPointX, int nbrPointY);

        void buildFaceCenter(int ownerPoint, int nbrPoint);

        void buildDeltaCoeffs();

        // Arrange patch information
        void buildPatch();


public:

    // Constructor

        // Construct from element number and mesh sizes
        orthoSquareMesh(int Nx, int Ny, double dx, double dy);

        // Construct from dictionary
        orthoSquareMesh(const Dictionary& dict);

        // Disallow bitwise copy
        orthoSquareMesh(const orthoSquareMesh&) = delete;


    // Member functions

        // Number of data
        int nPoints() const
        {
            return points_.size();
        }
        int nFaces() const
        {
            return faces_.size();
        }
        int nCells() const
        {
            return nCells_;
        }
        int nInternalFaces() const
        {
            return neighbor_.size();
        }

        // Return patch information
        const std::vector<Patch>& patches() const
        {
            return patches_;
        }
        const Patch& findPatch(std::string name) const;
        // Patch& patch(int i) const;

        // Reference of basic data
        const std::vector<Vector>& points() const
        {
            return points_;
        }
        const std::vector<std::vector<int>>& faces() const
        {
            return faces_;
        }
        const std::vector<int>& owner() const
        {
            return owner_;
        }
        const std::vector<int>& neighbor() const
        {
            return neighbor_;
        }

        // Reference of derived data
        const std::vector<Vector>& C() const
        {
            return C_;
        }
        const std::vector<double>& V() const
        {
            return V_;
        }
        const std::vector<Vector>& Cf() const
        {
            return Cf_;
        }
        const std::vector<Vector>& Sf() const
        {
            return Sf_;
        }
        const std::vector<double>& magSf() const
        {
            return magSf_;
        }
        const std::vector<double>& deltaCoeffs() const
        {
            return deltaCoeffs_;
        }
        // const std::vector<double>& weights() const;
        // const std::vector<Vector>& delta() const;
};


#endif