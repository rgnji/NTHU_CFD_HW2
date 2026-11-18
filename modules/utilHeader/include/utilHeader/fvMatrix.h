#ifndef fvMatrix_H
#define fvMatrix_H

#include "utility/lduMatrix.h"
#include "linearSolver/linearSolver.h"
#include "utility/pTraits.h"
#include <vector>


class orthoSquareMesh;

class Dictionary;

template<class Type>
class Field;

template<class Type>
class volField;


template<class Type>
class fvMatrix
:
    public lduMatrix
{
    // Private Data

        // Field reference
        volField<Type>& psi_;

        // Coefficients for boundary cells
        std::vector<Field<Type>> internalCoeffs_;

        // Coefficients for boundary faces
        std::vector<Field<Type>> boundaryCoeffs_;

        // Right-hand side without boundary contribution
        Field<Type> source_;


public:

    // Constructor

        // Allow manipulation of psi
        fvMatrix(volField<Type>& psi);
        

    // Member Functions

        // Return field
        const volField<Type>& psi() const
        {
            return psi_;
        }

        // Build internal coefficients
        void buildInternalCoeffs(int patch, int face, Type coeff);

        // Build boundary coefficients
        void buildBoundaryCoeffs(int patch, int face, Type coeff);

        // Add a value to the right-hand side of one cell
        void addSource(int cell, const Type& value)
        {
            source_.set(cell, source_.get(cell) + value);
        }

        // Reset the right-hand side (boundary coefficients are kept)
        void clearSource()
        {
            for (int k=0; k<source_.size(); k++)
                source_.set(k, pTraits<Type>::zero);
        }

        // Solve lduMatrix
        linearSolver::SolverPerformance solve(const linearSolver& solver);

        // Create solver from dictionary
        linearSolver::SolverPerformance solve(const Dictionary& dict);
};


#include "utilHeader/fvMatrix.tpp"


#endif