
#include "Line.h"

Line::Line(Vector2D start, Vector2D end)
{
    line_start = start;
    line_end   = end;
}

Line::~Line()
{
    
}

Vector2D &Line::getStartPoint()
{
    return line_start;
}

Vector2D &Line::getEndPoint()
{
    return line_end;
}

Vector2D* Line::intersectionPoint(Line& other)
{
    // Store the values for fast access and easy
    // equations-to-code conversion
    double x1 = getStartPoint().x, x2 = other.getStartPoint().x, x3 = getEndPoint().x, x4 = other.getEndPoint().x;
    double y1 = getStartPoint().y, y2 = other.getStartPoint().y, y3 = getEndPoint().y, y4 = other.getEndPoint().y;
    
    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    
    // If d is zero, there is no intersection
    if (d == 0) return 0;
    
    // Get the x and y
    double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
    double x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    double y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
    
    // Check if the x and y coordinates are within both lines
    if ( x < fmin(x1, x2) || x > fmax(x1, x2) ||
        x < fmin(x3, x4) || x > fmax(x3, x4) ) return 0;
    if ( y < fmin(y1, y2) || y > fmax(y1, y2) ||
        y < fmin(y3, y4) || y > fmax(y3, y4) ) return 0;
    
    // Return the point of intersection
    Vector2D* ret = new Vector2D();
    ret->x = x;
    ret->y = y;
    return ret;
}

const bool Line::doesLineIntersect(Line& other)
{
    return (intersectionPoint(other) != 0);
}