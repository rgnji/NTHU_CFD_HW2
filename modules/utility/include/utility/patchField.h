#ifndef PatchField_H
#define PatchField_H

#include "utility/Field.h"
#include <map>
#include <string>
#include <span>
#include <memory>


class Patch;

template<class Type>
class volField;

class Dictionary;


template<class Type>
class PatchField
:
    public Field<Type>
{
    // Private data

        // Reference to volField
        const volField<Type>& vol_;

        // Geometric information of patch
        const std::unique_ptr<Patch>& patch_;


public:

    // Constructor

        // Construct from mesh with patch name
        PatchField
        (
            const volField<Type>& vol,
            const Patch& patch,
            const Dictionary dict
        );

        // Copy constructor
        PatchField
        (
            const PatchField& oldField,
            const volField<Type>& internal
        );


    // Runtime Selection

        using tableType = std::unique_ptr<PatchField> (*) 
        (
            const volField<Type>& vol,
            const Patch& patch,
            const Dictionary dict
        );

        // Define table
        static std::map<std::string, tableType>& table()
        {
            static std::map<std::string, tableType> t;
            return t;
        }

        // Selector
        static std::unique_ptr<PatchField> New
        (
            const volField<Type>& vol,
            const Patch& patch,
            const Dictionary dict
        );

        // Registration
        template<class Derived>
        struct Register
        {
            explicit Register(const std::string& type)
            {
                table()[type] = []
                (
                    const volField<Type>& vol,
                    const Patch& patch,
                    const Dictionary dict
                ) -> std::unique_ptr<PatchField>
                {
                    return std::make_unique<Derived>(vol, patch, dict);
                }
            }
        };


    // Member Functions

        // Copy the object
        virtual std::unique_ptr<PatchField> clone
        (
            const volField<Type>& newField
        ) const = 0;

        // Return cell value next to the boundary face
        const Type patchInternalField(int faceIndex) const;
        
        // Update boundary value
        virtual void evaluate() = 0;

        // Return boundary face normal gradient
        virtual double snGrad() const = 0;

        // alpha
        virtual const double& valueInternalCoeffs() const = 0;
        
        // beta
        virtual const double& valueBoundaryCoeffs() const = 0;

        // gamma
        virtual const double& gradientInternalCoeffs(int) const = 0;

        // delta
        virtual const double& gradientBoundaryCoeffs(int) const = 0;
        
};


#include "utility/PatchField.tpp"


#endif