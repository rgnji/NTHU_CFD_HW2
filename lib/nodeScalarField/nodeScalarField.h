#ifndef nodeScalarField_H
#define nodeScalarField_H

#include "scalarField.h"

class nodeScalarField
:
    public scalarField
{
    // Mesh sizes (include boundary)
    int Nx_, Ny_;

    std::vector<int> internalNodeList_;


public:

    // Constructor

        // Construct from element number and mesh sizes
        nodeScalarField(int Nx, int Ny);


    // Member functions

        // Return mesh sizes
        int Nx() const;
        int Ny() const;

        // Return number of internal nodes
        int node() const;

        // Return number of internal faces
        int face() const;

        // Return internal node list
        const std::vector<int>& internalNodeList() const;
};


#endif