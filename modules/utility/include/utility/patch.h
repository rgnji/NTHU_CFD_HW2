#ifndef patch_H
#define patch_H

#include "utility/orthoSquareMesh.h"
#include <string>
#include <vector>
#include <span>


template<class Type>
class patch
{
    // Private data

        // Patch name
        std::string name_;

        // start face index
        int startFace_;

        // face number
        int nFaces_;

        // Reference to internal field
        std::vector<Type> internal_;

        // Owner slice correspond to the patch
        std::span<int> owner_;

        // Reciprocal of distance between cell and face center
        std::vector<double> deltaCoeff_;

    // Private Member Functions

        // Build deltaCoeff
        void buildDeltaCoeff(const orthoSquareMesh& mesh);


public:

    // Constructor

        patch
        (
            const orthoSquareMesh& mesh,
            const std::vector<Type>& internal,
            std::string name
        );

    // Member Functions

        // Access internal cells next to the boundary
        const std::span<const int> faceCells() const;

        // Return reciprocal of distance from cell center to face center
        const std::vector<double>& deltaCoeff() const;
};


#endif