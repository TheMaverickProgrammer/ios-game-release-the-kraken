#ifndef PREDATOR_H
#define PREDATOR_H

#include "Vector2D.h"
#include "Entity.h"

class Player;
class KrakenGameTiledMap;
class Camera;

class Predator : public Entity
{
private:
    Vector2D position;
    
public:
    // public member variables
    Player* player;
    Camera* cam;
    KrakenGameTiledMap *map;
    
    bool lock_clock;
    
    // public functions
    
    Predator() : Entity() { lock_clock = false; }
    ~Predator() { ; }
    
    Vector2D& getPosition() { return position; }
    
    virtual void onUpdate()   { ; }
    virtual void onDraw()     { ; }
    virtual void resetClock() { ; }
    
    void noticeMap(KrakenGameTiledMap *map)
    {
        this->map = map;
    }
    
    void noticeCamera(Camera* cam)
    {
        this->cam = cam;
    }
    
    void noticePlayer(Player* p)
    {
        player = p;
    }
};

#endif
