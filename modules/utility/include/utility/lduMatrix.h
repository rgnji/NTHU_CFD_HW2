/*
f         :  0  1  2  3  4  5  6
lowerAddr :  0  0  1  1  2  3  4
upperAddr :  1  3  2  4  5  4  5
upper     : ...
lower     : ...

losortAddr  :  0  2  1  3  5  4  6     ← arrange f according to upperAddr
losortStart :  0  0  1  2  3  5  7
*/


#ifndef lduMatrix_H
#define lduMatrix_H

#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include "utility/scalarField.h"
#include "utility/nodeScalarField.h"

class lduMatrix
{
public:

    // Possible direction for structured grids
    enum class direction {E,W,N,S,EE,WW,NN,SS,NE,NW,SE,SW};

    struct pairType
    {
        std::pair<int, int> couplings;
        lduMatrix::direction directions;

        bool operator<(const pairType& other) const
        {
            if (couplings.first != other.couplings.first)
                return couplings.first < other.couplings.first;
            return couplings.second < other.couplings.second;
        }

        bool operator==(const pairType& other) const
        {
            return 
                couplings.first == other.couplings.first &&
                couplings.second == other.couplings.second;
        }
    };


private:

    // Diagonal
    std::vector<double> diag_;

    // Lower triangle
    std::vector<double> lower_;

    // Upper triangle
    std::vector<double> upper_;

    // Righ hand side
    std::vector<double> b_;
    
    // Internal pairs

        // Lower triangle addressing
        std::vector<int> lowerAddr_;

        // Upper triangle addressing
        std::vector<int> upperAddr_;

        // Row pointer in CSR format for lower triangle
        std::vector<int> ownerStart_;

        std::vector<int> losortAddr_;

        std::vector<int> losortStart_;

        // Record the direction of the internal pairs
        std::vector<direction> pairDirection_;

    // Boundary pairs

        // Record the internal-boudary pairs
        std::vector<int> internalAddr_;

        std::vector<int> boundaryAddr_;

        std::vector<direction> boundaryDirection_;
    

public:

    // Constructor

        // Construct from field
        lduMatrix(const nodeScalarField& field);


    // Member functions

        // Build the addressing from field
        void buildLduAddressing
        (
            const nodeScalarField& phi,
            std::vector<pairType>& pairs
        );

        // Build addressing of internal-boudary pairs
        void buildBoundaryAddressing
        (
            const nodeScalarField& phi,
            std::vector<pairType>& boundaryPairs
        );

        // Build LDU coefficients
        void buildDiag(const int center, const double coeff);
        void buildLower(const int f, const double coeff);
        void buildUpper(const int f, const double coeff);
        void buildB(const int f, const double coeff);

        // Return coefficients
        const std::vector<double>& diag() const;
        const std::vector<double>& lower() const;
        const std::vector<double>& upper() const;
        const std::vector<double>& b() const;

        // Return addressing
        const std::vector<int>& lowerAddr() const;
        const std::vector<int>& upperAddr() const;
        const std::vector<int>& losortAddr() const;

        // Return start
        const std::vector<int>& ownerStart() const;
        const std::vector<int>& losortStart() const;

        // Return pairDirection_
        const std::vector<direction>& pairDirection() const;

        const std::vector<int>& internalAddr() const;

        const std::vector<int>& boundaryAddr() const;

        const std::vector<direction>& boundaryDirection() const;

        // Calculate residual
        double residualNorm(const nodeScalarField& phi) const;
};


#endif