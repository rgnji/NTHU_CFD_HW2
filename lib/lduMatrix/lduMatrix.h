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
    // Diagonal
    std::vector<double> diag_;

    // Upper triangle
    std::vector<double> upper_;

    // Lower triangle
    std::vector<double> lower_;

    // Upper triangle addressing
    std::vector<int> upperAddr_;

    // Lower triangle addressing
    std::vector<int> lowerAddr_;

    // Row pointer in CSR format for lower triangle
    std::vector<int> ownerStart_;

    std::vector<int> losortAddr_;

    std::vector<int> losortStart_;


public:

    // Constructor

        // Construct from field
        lduMatrix(const nodeScalarField field);


    // Member functions

        // Build the addressing from field
        void buildLduAddressing
        (
            const nodeScalarField phi,
            const std::vector<std::pair<int,int>> couplings
        );
};


#endif