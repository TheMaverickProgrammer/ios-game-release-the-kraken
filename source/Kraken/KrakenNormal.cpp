//
//  KrakenNormal.cpp
//  Kraken
//
//  Created by Maverick Programmer on 5/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "KrakenNormal.h"

KrakenNormal::KrakenNormal()
{
    speed = 0.1;
    getPosition() = Vector2D(-20, 8*8);
    
    all_my_arms.add(new KrakenTentacle(6, this, Vector2D(32, 10)));
    all_my_arms.add(new KrakenTentacle(10, this, Vector2D(32, 18)));
    all_my_arms.add(new KrakenTentacle(10, this, Vector2D(32, 26)));
    all_my_arms.add(new KrakenTentacle(6, this, Vector2D(32, 34)));
    
    kraken_begin_time = Gosu::milliseconds();
    last              = kraken_begin_time;
}

KrakenNormal::~KrakenNormal()
{
    
}

bool KrakenNormal::hasTentacleHitPlayer(Player* player)
{
    for(int i = 0; i < all_my_arms.getSize(); i++)
    {
        KrakenTentacle *ptr = all_my_arms.get(i);
        
        Vector2D coord;
        Vector2D pos = getPosition() + ptr->getPosition();
        
        while( (ptr->hasChild() ) )
        {
            // draw arm            
            Vector2D coord = pos;
            pos = pos + ptr->getDrawPosition();
            
            ptr = ptr->getChild();
        }
        
        pos = pos + ptr->getDrawPosition();
        
        // We have the end tentacle
        if(player->isPointInside(pos))
        {
            return true;
        }
    }
    
    return false;
}

void KrakenNormal::onUpdate()
{
    for(int i = 0; i < all_my_arms.getSize(); i++)
        all_my_arms.get(i)->update();
    
    static double tinker = 1.0;
    
    getPosition().x += speed;
    
    speed *= 0.98;
    
    if( (Gosu::milliseconds()-kraken_begin_time)/1000 - last/1000 > 3)
    {
        tinker = fabs(50 + getPosition().x - player->getX())/40.0;
        last = (Gosu::milliseconds()-kraken_begin_time);
        speed+=tinker;
    }
    
    if(player->getX() <= getPosition().x + 50)
    {
        player->signalGameOver();
    }
    
    if(speed <= 0.5)
        speed = 0.5;
}

void KrakenNormal::onDraw()
{
    Vector2D coord = cam->worldToScreen(getPosition());
    double   scale = cam->getZoom();
    
    // draw arms first
    for(int i = 0; i < all_my_arms.getSize(); i++)
        all_my_arms.get(i)->draw(cam);
    
    // then body so body is on top
    if(Core::get().getGraphicResources().contains("KrakenBody"))
    {
        Core::get().getGraphicResources().get("KrakenBody")->draw(coord.x, coord.y, 5, scale, scale);
    }
    
}

void KrakenNormal::resetClock()
{
    last = (Gosu::milliseconds()-kraken_begin_time); 
}