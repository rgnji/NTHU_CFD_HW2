#ifndef Patch_H
#define Patch_H

#include "utility/orthoSquareMesh.h"
#include "utility/Vector.h"
#include <string>
#include <vector>
#include <span>


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
        
        // Return mesh
        const orthoSquareMesh& mesh() const;

        // Access internal cells next to the boundary
        const std::span<const int> faceCells() const;

        // Return reciprocal of distance from cell center to face center
        const std::vector<double>& deltaCoeffs() const;
};


#endif