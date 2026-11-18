#ifndef pTraits_H
#define pTraits_H

#include "utility/Vector.h"


template<class Type> struct pTraits;

template<> struct pTraits<double>
{
    static constexpr double zero = 0.0;
    static constexpr double one = 1.0;
};

template<> struct pTraits<Vector>
{
    static constexpr Vector zero{0.0, 0.0};
    static constexpr Vector one{1.0, 1.0};
};


#endif