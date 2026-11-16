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
        const Patch& patch_;


public:

    // Constructor

        // Construct from dictionary
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


    // Destructor
    virtual ~PatchField() = default;


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
            const Dictionary& dict
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
                };
            }
        };


    // Member Functions

        // Copy the object
        virtual std::unique_ptr<PatchField> clone
        (
            const volField<Type>& newField
        ) const = 0;

        // Return cell value next to the boundary face
        Field<Type> patchInternalField() const;
        
        // Update boundary value
        virtual void evaluate() {};

        // Return boundary face normal gradient
        virtual Field<Type> snGrad() const;

        // alpha
        virtual Field<Type> valueInternalCoeffs() const = 0;
        
        // beta
        virtual Field<Type> valueBoundaryCoeffs() const = 0;

        // gamma
        virtual Field<Type> gradientInternalCoeffs() const = 0;

        // delta
        virtual Field<Type> gradientBoundaryCoeffs() const = 0;

        // Return volField
        const volField<Type>& vol() const
        {
            return vol_;
        }

        // Return patch
        const Patch& patch() const
        {
            return patch_;
        }
};


#include "utility/PatchField.tpp"


#define makePatchFields(Name)                      \
    namespace                                      \
    {                                              \
        PatchField<double>::Register<Name<double>> \
            add##Name##ScalarPatchField(#Name);    \
        PatchField<Vector>::Register<Name<Vector>> \
            add##Name##VectorPatchField(#Name);    \
    }


#endif