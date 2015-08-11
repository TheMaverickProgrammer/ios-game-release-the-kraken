#ifndef LINE_H
#define LINE_H

#include "../Vector2D.h"

class Line 
{
private:
    Vector2D line_start;
    Vector2D line_end;
    
public:
    
    Vector2D &getStartPoint();
    Vector2D &getEndPoint();
    
    Line(Vector2D start, Vector2D end);
    ~Line();
    
    Vector2D* intersectionPoint(Line& other);
    const bool doesLineIntersect(Line& other);
};

#endif