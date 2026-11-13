#ifndef PatchField_H
#define PatchField_H

#include "utility/Field.h"
#include "utility/Patch.h"
#include "utility/volField.h"
#include <map>
#include <string>
#include <span>
#include <memory>


template<class Type>
class PatchField
:
    public Field<Type>
{
    // Private data

        // Reference to volField
        volField<Type>& vol_;

        // Geometric information of patch
        Patch& patch_;

        // phi_b = alpha_ * phi_P + beta_
        double alpha_;
        double beta_;

        // Face normal gradient = gamma_ * phi_P + delta
        double gamma_;
        double delta_;


public:

    using SubDict = std::map<std::string, std::string>;

    // Constructor

        // Construct from mesh with patch name
        PatchField
        (
            const volField<Type>& vol,
            const Patch& patch,
            const SubDict& dict
        );


    // Member Functions

        // Return cell value next to the boundary face
        const Type patchInternalField(int faceIndex) const;
        
        // Update boundary value
        void evaluate() = 0;

        // Return boundary face normal gradient
        double snGrad() const = 0;


    // Runtime Selection

        using tableType = std::unique_ptr<PatchField> (*) 
        (
            const orthoSquareMesh& mesh, 
            const std::vector<Type>& internal, 
            const std::string name
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
            const orthoSquareMesh& mesh,
            const std::vector<Type>& internal,
            const std::string name
        );

        // Registration
        template<class Derived>
        struct Register
        {
            explicit Register(const std::string& type)
            {
                table()[type] = []
                (
                    const orthoSquareMesh& mesh, 
                    const std::vector<Type>& internal, 
                    const std::string name
                ) -> std::unique_ptr<PatchField>
                {
                    return std::make_unique<Derived>(mesh, internal, name);
                }
            }
        }
};


#endif