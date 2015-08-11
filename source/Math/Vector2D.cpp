#include "Vector2D.h"

Vector2D::Vector2D()
{
    zero();
}

Vector2D::Vector2D(double x, double y)
{
    this->x = x;
    this->y = y;
}

const Vector2D& Vector2D::operator=(const Vector2D &rhs)
{
    if(this != &rhs)
    {
        this->x = rhs.x;
        this->y = rhs.y;
    }
    
    return *this;
}

Vector2D::~Vector2D()
{ ; }

void Vector2D::zero()
{
    x = 0.0;
    y = 0.0;
}

bool Vector2D::isZero()
{
    return (x == 0.0 && y == 0.0);
}

double Vector2D::length()
{
    return sqrt(x * x + y * y);
}

double Vector2D::lengthSquared()
{
    return (x * x + y * y);   
}

void Vector2D::normalize()
{
    double vector_length = length();
    
    if(vector_length > std::numeric_limits<double>::epsilon())
    {
        x /= vector_length;
        y /= vector_length;
    }
}

double Vector2D::dot(const Vector2D& other) const
{
    return (x*other.x + y*other.y);
}

enum {clockwise = 1, anticlockwise = -1};
int Vector2D::sign (const Vector2D& other) const
{
    if(y*other.x > x*other.y)
    {
        return anticlockwise;
    }
    else
    {
        return clockwise;
    }
}

Vector2D Vector2D::perpendicular() const
{
    return Vector2D(-y, x);
}

void Vector2D::truncate(double max)
{
    if(length() > max)
    {
        normalize();
        *this *= max; /** multiply vector to a scalar **/
    }
}

double Vector2D::distance(const Vector2D& other)const
{
    double y_diff = other.y - y;
    double x_diff = other.x - x;
    
    return sqrt(y_diff*y_diff + x_diff*x_diff);
}

double Vector2D::distanceSquared(const Vector2D& other) const
{
    double y_diff = other.y - y;
    double x_diff = other.x - x;
    
    return (y_diff*y_diff + x_diff*x_diff);    
}

Vector2D& Vector2D::reflect(const Vector2D& norm)
{
    *this += 2.0 * dot(norm) * norm.reverse();   
    
    return *this;
}

Vector2D Vector2D::reverse() const
{
    return (Vector2D(-x, -y));
}

const Vector2D& Vector2D::operator+=(const Vector2D& other)
{
    x += other.x;
    y += other.y;
    
    return *this;
}

const Vector2D& Vector2D::operator-=(const Vector2D& other)
{
    x -= other.x;
    y -= other.y;
    
    return *this;  
}

const Vector2D& Vector2D::operator*=(const double& scalar)
{
    x *= scalar;
    y *= scalar;
    
    return *this; 
}

const Vector2D& Vector2D::operator/=(const double& scalar)
{
    x /= scalar;
    y /= scalar;
    
    return *this;     
}

const bool Vector2D::operator==(const Vector2D& other) const
{
    return(x == other.x && y == other.y);
}

const bool Vector2D::operator!=(const Vector2D& other) const
{
    return !(*this == other);
}

Vector2D operator*(const Vector2D& lhs, double rhs)
{
    Vector2D result(lhs);
    result *= rhs;
    return result;
}

Vector2D operator*(double lhs, const Vector2D& rhs)
{
    Vector2D result(rhs);
    result *= lhs;
    return result;
}

Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs)
{
    Vector2D result(lhs);
    
    result.x -= rhs.x;
    result.y -= rhs.y;

    return result;
}

Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
    Vector2D result(lhs);
    
    result.x += rhs.x;
    result.y += rhs.y;
    
    return result;
}

Vector2D operator/(const Vector2D &lhs, double val)
{
    Vector2D result(lhs);
    result.x /= val;
    result.y /= val;
    
    return result;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& rhs)
{
    os << " " << rhs.x << " " << rhs.y;
    
    return os;
}


std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs)
{
    is >> lhs.x >> lhs.y;
    
    return is;
}
