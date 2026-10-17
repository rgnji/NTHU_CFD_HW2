/*
原本的資料（只存一份，按 owner 排序）：
f         :  0  1  2  3  4  5  6
lowerAddr :  0  0  1  1  2  3  4
upperAddr :  1  3  2  4  5  4  5
upper     : ...
lower     : ...

額外建立的索引（只存 f）：
losortAddr  :  0  2  1  3  5  4  6     ← 把 f 按 upperAddr 排序
losortStart :  0  0  1  2  3  5  7
*/


#ifndef lduMatrix_H
#define lduMatrix_H

#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include "scalarField.h"
#include "nodeScalarField.h"

class lduMatrix
{
public:

    // Possible direction for structured grids
    const static enum class direction {E,W,N,S,EE,WW,NN,SS};

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
    
    // Lower triangle addressing
    std::vector<int> lowerAddr_;

    // Upper triangle addressing
    std::vector<int> upperAddr_;

    // Row pointer in CSR format for lower triangle
    std::vector<int> ownerStart_;

    std::vector<int> losortAddr_;

    std::vector<int> losortStart_;

    // Record the direction of the pairs
    std::vector<direction> pairDirection_;
    

public:

    // Constructor

        // Construct from field
        lduMatrix(const nodeScalarField& field);


    // Member functions

        // Build the addressing from field
        void buildLduAddressing
        (
            const nodeScalarField& phi,
            std::vector<pairType> pairs
        );

        // Build LDU coefficients
        void buildDiag(const int center, const double coeff);
        void buildLower(const int f, const double coeff);
        void buildUpper(const int f, const double coeff);

        // Return coefficients
        const std::vector<double>& diag() const;
        const std::vector<double>& lower() const;
        const std::vector<double>& upper() const;

        // Return addressing
        const std::vector<int>& lowerAddr() const;
        const std::vector<int>& upperAddr() const;

        // Return start
        const std::vector<int>& ownerStart() const;
        const std::vector<int>& losortStart() const;

        // Return pairDirection_
        const std::vector<direction>& pairDirection() const;
};


#endif