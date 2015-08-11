#ifndef TRIGGER_H
#define TRIGGER_H

#include "EventPublisher.h"
#include "Vector2D.h"

template<typename EVENT_TYPE>
class Trigger : public EventPublisher<EVENT_TYPE>
{
private:
    Vector2D position;
    int height;
    int width;
    
public:
    Trigger(Vector2D pos, int w, int h)
    {
        position = pos;
        height   = h;
        width    = w;
    }
    
    const bool isPointInBounds(Vector2D point)
    {
        return ( point.x <= pos.x + w && point.x >= pos.x && point.y >= pos.y + h && point.y <= pos.y );
    }
};

#endif