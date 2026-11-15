#ifndef zeroGradient_H
#define zeroGradient_H

#include "utility/PatchField.h"


template<class Type>
class zeroGradient
:
    public PatchField<Type>
{
public:

    // Constructor

        // Construct from dictionary
        zeroGradient
        (
            const volField<Type>& vol,
            const Patch& patch,
            const Dictionary dict
        );

        // Copy constructor
        zeroGradient
        (
            const zeroGradient& oldField,
            const volField<Type>& internal
        );


    // Member Functions

        // Copy the object
        virtual std::unique_ptr<zeroGradient> clone
        (
            const volField<Type>& newField
        ) const;

        // Update boundary value
        virtual void evaluate() override;

        // Return boundary face normal gradient
        virtual Field<Type> snGrad() const override;

        // alpha
        virtual Field<Type> valueInternalCoeffs() const;
        
        // beta
        virtual Field<Type> valueBoundaryCoeffs() const;

        // gamma
        virtual Field<Type> gradientInternalCoeffs() const;

        // delta
        virtual Field<Type> gradientBoundaryCoeffs() const;
};


#include "boundaryCondition/zeroGradient.tpp"


#endif