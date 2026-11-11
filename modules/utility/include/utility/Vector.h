#ifndef Vector_H
#define Vector_H

struct Vector
{
    double x,y;

    Vector operator+(const Vector& other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector operator-(const Vector& other) const
    {
        return {x - other.x, y - other.y};
    }

    double operator&(const Vector& other) const
    {
        return x * other.x + y * other.y;
    }

    Vector operator*(double factor) const
    {
        return {x * factor, y * factor};
    }
};


#endif