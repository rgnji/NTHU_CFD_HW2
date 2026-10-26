#ifndef scalarField_H
#define scalarField_H

#include <vector>

class scalarField
{
    // Element number
    int N_;

    // 1D field object
    std::vector<double> field_;


public:

    // Constructor

        // Construct from element number
        scalarField(int N);

    // Member functions

        // Return total element number
        int total() const;

        // Setter
        void setValue(int N, const double value);

        // Getter
        double getValue(int N) const;
};


#endif