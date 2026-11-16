#ifndef fixedValue_H
#define fixedValue_H

#include "utility/PatchField.h"
#include <string>
#include <vector>


class Dictionary;

class Patch;

template<class Type>
class volField;


template<class Type>
class fixedValue
:
    public PatchField<Type>
{
    // Private data

public:

    // Constructor

        // Construct from dictionary
        fixedValue
        (
            const volField<Type>& vol,
            const Patch& patch,
            const Dictionary dict
        );

        // Copy constructor
        fixedValue
        (
            const fixedValue& oldField,
            const volField<Type>& internal
        );


    // Member Functions

        // Copy the object
        virtual std::unique_ptr<fixedValue> clone
        (
            const volField<Type>& newField
        ) const override;

        // alpha
        virtual Field<Type> valueInternalCoeffs() const override;

        // beta
        virtual Field<Type> valueBoundaryCoeffs() const override;

        // gamma
        virtual Field<Type> gradientInternalCoeffs() const override;

        // delta
        virtual Field<Type> gradientBoundaryCoeffs() const override;
};


#include "boundaryCondition/fixedValue.tpp"


#endif