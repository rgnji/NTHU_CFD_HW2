#ifndef Patch_H
#define Patch_H

#include <string>
#include <vector>
#include <span>


class orthoSquareMesh;


class Patch
{
    // Private data

        // Reference to mesh
        const orthoSquareMesh& mesh_;

        // Patch name
        std::string name_;

        // start face index
        int startFace_;

        // face number
        int nFaces_;

        // Reciprocal of distance between cell and face center
        std::vector<double> deltaCoeff_;

    // Private Member Functions

        // Build deltaCoeff
        void buildDeltaCoeff(const orthoSquareMesh& mesh);


public:

    // Constructor

        // Construct from components
        Patch
        (
            const orthoSquareMesh& mesh,
            const std::string name,
            const int startFace,
            const int nFaces
        );


    // Member Functions

        // Return private data
        const std::string& name() const
        {
            return name_;
        }
        const orthoSquareMesh& mesh() const
        {
            return mesh_;
        }
        const int& startFace() const
        {
            return startFace_;
        }
        const int& nFaces() const
        {
            return nFaces_;
        }

        // Return reciprocal of distance from cell center to face center
        const std::vector<double>& deltaCoeffs() const
        {
            return deltaCoeff_;
        }
        
        // Access internal cells next to the boundary
        const std::span<const int> faceCells() const;

        // Return internal cell index according to face index
        const int faceCells(int f) const
        {
            return faceCells()[f];
        }
};


#endif