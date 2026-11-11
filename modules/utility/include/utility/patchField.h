#ifndef patchField_H
#define patchField_H

#include "utility/Vector.h"
#include "utility/Field.h"
#include "utility/patch.h"
#include "utility/volField.h"
#include "utility/orthoSquareMesh.h"
#include <string>
#include <span>


template<class Type>
class patchField
:
    public Field<Type>,
    public patch<Type>
{
    // Private data

        // phi_b = alpha_ * phi_P + beta_
        double alpha_;
        double beta_;

        // Face normal gradient = gamma_ * phi_P + delta
        double gamma_;
        double delta_;


public:

    // Constructor

        // Construct from mesh with patch name
        patchField
        (
            const orthoSquareMesh& mesh, 
            const std::vector<Type>& internal,
            std::string name
        );

    // Member Functions

        // Return cell value next to the boundary face
        const Type patchInternalField(int faceIndex) const;
        
        // Update boundary value
        void evaluate() = 0;

        // Return boundary face normal gradient
        double snGrad() const = 0;
};


#endif