//
//  KrakenNormal.h
//  Kraken
//
//  Created by Maverick Programmer on 5/27/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KRAKEN_NORMAL_H
#define KRAKEN_NORMAL_H

#include "Entity.h"
#include "KrakenGameTiledMap.h"
#include "KrakenGame.h"
#include "Camera.h"
#include "Player.h"
#include "Core.h"
#include "Vector2D.h"
#include "Group.h"
#include "Predator.h"

class KrakenTentacle;

class KrakenNormal : public Entity, public Predator
{
private:
    Vector2D position;
    long    kraken_begin_time;
    long    last;
    
    double speed;
    
     class KrakenTentacle // Gets drawn in Kraken onDraw() function
     {
     private:
         KrakenTentacle* child; // null means this is end
         
         double current_angle;
         double delta_angle;
         bool is_end;
         
         
         KrakenNormal *kraken;
         
         Vector2D position;
     public:
         
         KrakenTentacle(int length, KrakenNormal *kraken, Vector2D position)
         {
             if(length != 0)
             {
                 is_end = false;
                 child = new KrakenTentacle(length-1, kraken, position);
             }
             else
             {
                 is_end = true;
                 child = 0; // no child; endpoint
             }
             
             current_angle = 0;
             delta_angle   = 2;
             
             this->position = position;
             
             this->kraken = kraken;
         }
         
         Vector2D getPosition()
         {
             return position;
         }
         
         void update()
         {
             if(Gosu::random(0, 100) > 95)
                 delta_angle = Gosu::random(-2.0, 2.0);
             
             current_angle += delta_angle;
             
             if(current_angle <= -45 || current_angle >= 45)
                 delta_angle = -delta_angle;
             
             if( hasChild() )
             {
                 getChild()->update();
             }
         }
         
         const bool   hasChild() { return !is_end; }
         
         KrakenTentacle* getChild() { return child; }
         
         const double getCurrentAngle() { return current_angle; }
         
         void attackPoint(Vector2D point)
         {
            const double RADIANS = (22.0/7.0)/180.0; // PI / 180
             Vector2D pos(cosf(current_angle*RADIANS)*32, sinf(current_angle*RADIANS)*32);
             pos.normalize();
             point.normalize();
             
             current_angle += acos(pos.dot(point));
             
             if(hasChild())
                 getChild()->attackPoint(point);
         }
         
         void returnFromAttack()
         {
             current_angle = 0;
             
             if(hasChild())
                 getChild()->returnFromAttack();
         }
         
         Vector2D getDrawPosition()
         {
             Vector2D return_result;
             
             const double RADIANS = (22.0/7.0)/180.0; // PI / 180
             return_result = Vector2D( 32*cosf(current_angle*RADIANS), 32*sinf(current_angle*RADIANS));
             
             return return_result;
         }
         
         void draw(Camera* cam)
         {
                 KrakenTentacle *ptr = this;
                 
                 int i = 0;
                 
                 Vector2D coord;
                 Vector2D draw_pos = cam->worldToScreen(kraken->getPosition() + position);
                 
                 while( (ptr->hasChild() ) )
                 {
                     // draw arm            
                     Vector2D coord = draw_pos;
                     draw_pos = draw_pos + ptr->getDrawPosition();
                     
                     double   scale = cam->getZoom();
                     Core::get().getGraphicResources().get("KrakenArm")->drawRot(coord.x, coord.y, 5, ptr->getCurrentAngle(), 0, 0.5, scale, scale);
                     
                     ptr = ptr->getChild();
                     
                     i++;
                 }
                 
                 // otherwise draw end
                 coord = draw_pos;
                 double   scale = cam->getZoom();
                 Core::get().getGraphicResources().get("KrakenEnd")->drawRot(coord.x, coord.y, 5, ptr->getCurrentAngle(), 0, 0.5, scale, scale);
         }
     };
    
    Group<KrakenTentacle*> all_my_arms;
    
public:
    KrakenNormal();
    ~KrakenNormal();
    
    void onUpdate();
    void onDraw();
    void resetClock();
    
    bool hasTentacleHitPlayer(Player* player);
};

#endif