#ifndef QUICK_H
#define QUICK_H

#include "convectionScheme/convectionScheme.h"
#include "field/surfaceField.h"
#include "field/Field.h"
#include "utility/Vector.h"
#include <type_traits>


template<class Type>
class volField;


namespace convectionSchemeType
{

template<class Type>
class QUICK
:
    public convectionScheme<Type>
{
    static_assert
    (
        std::is_same_v<Type, double>,
        "QUICK is implemented for scalar fields only"
    );


    // Private Member Functions

        // Cell gradient by the Gauss theorem (linear face interpolation)
        Field<Vector> gaussGradient(const volField<Type>& vf) const;

        // QUICK face values on all faces (boundary faces from the patch fields)
        surfaceField<Type> faceValue
        (
            const volField<Type>& vf,
            const surfaceField<double>& phi
        ) const;

        // Add F_f * (phi_f^QUICK - phi_f^upwind) to the source of eqn
        void addCorrection
        (
            fvMatrix<Type>& eqn,
            const surfaceField<double>& phi
        ) const;


public:

    // Member functions

        // Implicit part: upwind weights
        virtual surfaceField<double> weights
        (
            const surfaceField<double>& phi
        ) const override;

        // Explicit part: QUICK correction
        virtual void correct
        (
            fvMatrix<Type>& eqn,
            const surfaceField<double>& phi
        ) const override;
};
}


#include "convectionScheme/QUICK.tpp"


#endif