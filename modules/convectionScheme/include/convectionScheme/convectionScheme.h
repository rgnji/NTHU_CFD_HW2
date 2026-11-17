#ifndef convectionScheme_H
#define convectionScheme_H

#include "field/surfaceField.h"
#include <map>
#include <memory>
#include <string>


class Dictionary;

template<class Type>
class fvMatrix;


template<class Type>
class convectionScheme
{
public:

    // Destructor
    virtual ~convectionScheme() = default;

    // Runtime Selection

        using tableType = std::unique_ptr<convectionScheme>(*) ();

        // Define table
        static std::map<std::string, tableType>& table()
        {
            static std::map<std::string, tableType> t;
            return t;
        }

        // Selector
        static std::unique_ptr<convectionScheme> New
        (
            const Dictionary& dict
        );

        // Registration
        template<class Derived>
        struct Register
        {
            explicit Register(const std::string& type)
            {
                table()[type] = [] () -> std::unique_ptr<convectionScheme>
                {
                    return std::make_unique<Derived>();
                };
            }
        };

    // Member Functions

        // Interpolation weights
        virtual surfaceField<double> weights
        (
            const surfaceField<double>&
        ) const = 0;

        // Generate fvMatrix
        void add
        (
            fvMatrix<Type>& eqn,
            const surfaceField<double>& phi,
            const surfaceField<double>& weights
        );
};


#include "convectionScheme/convectionScheme.tpp"


#endif