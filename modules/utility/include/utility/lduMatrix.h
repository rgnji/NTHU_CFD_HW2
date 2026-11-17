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
#include <span>


class orthoSquareMesh;


class lduMatrix
{
    // Private data

        // Number of internal faces
        int nFaces_;

        // Coefficients

            // Diagonal
            std::vector<double> diag_;

            // Lower triangle
            std::vector<double> lower_;

            // Upper triangle
            std::vector<double> upper_;

        // Internal pairs

            // Lower triangle addressing
            const std::vector<int>& owner_;

            // Upper triangle addressing
            const std::vector<int>& neighbor_;

            // Row pointer in CSR format for lower triangle
            std::vector<int> ownerStart_;

            std::vector<int> losortAddr_;

            std::vector<int> losortStart_;


    // Private Member Functions

        // Build the addressing from field
        void buildLduAddressing();
    

public:

    // Constructor

        // Construct from field
        lduMatrix(const orthoSquareMesh& mesh);


    // Member functions

        // Build LDU coefficients
        void buildDiag(int center, double coeff);
        void buildLower(int f, double coeff);
        void buildUpper(int f, double coeff);

        // Return coefficients
        const std::vector<double>& diag() const
        {
            return diag_;
        }
        std::vector<double>& diagList()
        {
            return diag_;
        }
        const std::vector<double>& lower() const
        {
            return lower_;
        }
        const std::vector<double>& upper() const
        {
            return upper_;
        }

        // Return addressing
        std::span<const int> lowerAddr() const
        {
            return std::span<const int>(owner_.data(), neighbor_.size());
        }
        const std::vector<int>& upperAddr() const
        {
            return neighbor_;
        }
        const std::vector<int>& losortAddr() const
        {
            return losortAddr_;
        }

        // Return start
        const std::vector<int>& ownerStart() const
        {
            return ownerStart_;
        }
        const std::vector<int>& losortStart() const
        {
            return losortStart_;
        }

        // Calculate residual
        std::vector<double> residual
        (
            const std::vector<double>& x,
            const std::vector<double>& b
        ) const;
        double residualNorm
        (
            const std::vector<double>& x,
            const std::vector<double>& b
        ) const;
};


#endif