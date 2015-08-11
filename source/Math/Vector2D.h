#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

const double PI     = 22.0/7.0;
const double TWO_PI = 2 * PI;

#include <math.h>
#include <fstream>
#include <limits>

class Vector2D
{
public:
    double x, y;
    
    Vector2D();
    Vector2D(double, double);
    
    ~Vector2D();
    
    void zero();
    bool isZero();
    
     double length();
     double lengthSquared();
     void   normalize();
     double dot(const Vector2D&) const;
     int sign (const Vector2D&) const;
     Vector2D perpendicular() const;
     void truncate(double max);
     double distance(const Vector2D&)const;
     double distanceSquared(const Vector2D&) const;
     Vector2D &reflect(const Vector2D&);
     Vector2D reverse() const;
    
    const Vector2D& operator+=(const Vector2D&);
    const Vector2D& operator-=(const Vector2D&);
    const Vector2D& operator*=(const double&);
    const Vector2D& operator/=(const double&);
    const Vector2D& operator= (const Vector2D&);
    
    const bool operator==(const Vector2D&) const;
    const bool operator!=(const Vector2D&) const;
};

Vector2D operator*(const Vector2D &, double);
Vector2D operator*(double, const Vector2D&);
Vector2D operator-(const Vector2D&, const Vector2D&);
Vector2D operator+(const Vector2D &, const Vector2D&);
Vector2D operator/(const Vector2D &, double);
std::ostream& operator<<(std::ostream&, const Vector2D&);
std::ifstream& operator>>(std::ifstream&, Vector2D&);

#endif
