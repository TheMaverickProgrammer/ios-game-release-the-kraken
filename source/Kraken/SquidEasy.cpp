//
//  SquidEasy.cpp
//  Squid
//
//  Created by Maverick Programmer on 5/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SquidEasy.h"

SquidEasy::SquidEasy()
{
    speed = 0.1;
    getPosition() = Vector2D(-20, 9*8);
    
    all_my_arms.add(new SquidTentacle(2, this, Vector2D(20, 14)));
    all_my_arms.add(new SquidTentacle(3, this, Vector2D(20, 16)));
    all_my_arms.add(new SquidTentacle(2, this, Vector2D(20, 18)));
    
    squid_begin_time = Gosu::milliseconds();
    last             = squid_begin_time;
}

SquidEasy::~SquidEasy()
{
    
}

bool SquidEasy::hasTentacleHitPlayer(Player* player)
{
     return false;
}

void SquidEasy::onUpdate()
{
    for(int i = 0; i < all_my_arms.getSize(); i++)
        all_my_arms.get(i)->update();
    
    static double tinker = 1.0;
    
    getPosition().x += speed;
    
    speed *= 0.98;
    
    if( (Gosu::milliseconds()-squid_begin_time)/1000 - last/1000 > 1)
    {
        tinker = fabs(50 + getPosition().x - player->getX())/50.0;
        last = (Gosu::milliseconds()-squid_begin_time);
        speed+=tinker;
    }
    
    if(player->getX() <= getPosition().x + 30)
    {
        if(getPosition().y > player->getY() - 10)
            getPosition().y -= 2.2;
        else if(getPosition().y < player->getY() - 22)
            getPosition().y += 2.2;
        else
            player->signalGameOver();
    }
    
    if(getPosition().y > player->getY() - 16)
        getPosition().y -= 0.2;
    else if(getPosition().y < player->getY() - 16)
        getPosition().y += 0.2;
    
    if(speed <= 0.5)
        speed = 0.5;
}

void SquidEasy::onDraw()
{
    Vector2D coord = cam->worldToScreen(getPosition());
    double   scale = cam->getZoom();
    
    // draw arms first
    for(int i = 0; i < all_my_arms.getSize(); i++)
        all_my_arms.get(i)->draw(cam);
    
    // then body so body is on top
    if(Core::get().getGraphicResources().contains("SquidBody"))
    {
        Core::get().getGraphicResources().get("SquidBody")->draw(coord.x, coord.y, 5, scale, scale);
    }
    
}

void SquidEasy::resetClock() 
{
    last = (Gosu::milliseconds()-squid_begin_time); 
}