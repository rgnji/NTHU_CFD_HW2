#ifndef nodeScalarField_H
#define nodeScalarField_H

#include "utility/scalarField.h"

class nodeScalarField
:
    public scalarField
{
    // Mesh sizes (include boundary)
    int Nx_, Ny_;


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

        // Convert global to local(no boundary included)
        int internalConvert(int p) const;
        
        // Convert local(no boundary included) to global
        int globalIndex(int P) const;
};


#endif