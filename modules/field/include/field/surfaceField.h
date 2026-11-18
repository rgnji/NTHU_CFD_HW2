#ifndef surfaceField_H
#define surfaceField_H

#include "field/Field.h"
#include "utility/orthoSquareMesh.h"
#include <vector>


template<class Type>
class surfaceField
:
    public Field<Type>
{
    // Private Data

        // Mesh reference
        const orthoSquareMesh& mesh_;


public:

    // Constructor

        // Construct from mesh
        surfaceField(const orthoSquareMesh& mesh);

        // Construct from mesh and initial value
        surfaceField(const orthoSquareMesh& mesh, Type value);

        // Construct from mesh and field
        surfaceField
        (
            const orthoSquareMesh& mesh, 
            const std::vector<Type>& field
        );


    // Member Functions

        // Return mesh object
        const orthoSquareMesh& mesh() const
        {
            return mesh_;
        }
};


// Global operator overload

    // Inner product of two Vector surfaceField
    inline surfaceField<double> operator&
    (
        const surfaceField<Vector>& a,
        const surfaceField<Vector>& b
    )
    {
        surfaceField<double> result(a.mesh(), 0.0);

        for (int f=0; f<a.mesh().nFaces(); f++)
            result.set(f, a.get(f) & b.get(f));

        return result;
    }


#include "field/surfaceField.tpp"


#endif