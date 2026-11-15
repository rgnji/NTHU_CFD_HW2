#ifndef fvMatrix_H
#define fvMatrix_H

#include "utility/lduMatrix.h"
#include <vector>


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
};


#include "utility/fvMatrix.tpp"


#endif