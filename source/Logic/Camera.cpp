/*
 *  Camera.cpp
 *
 *  Created by The Maverick Programmer on 7/19/09.
 *  Copyright 2009 The Maverick Programmer. All rights reserved.
 *
 */

#include "Camera.h"

const double EPSILON = 0.0001;

void Camera::setWorldDimensions(const int width, const int height)
{
    levelWidth = width;
    levelHeight = height;
}

/*
    Keep in mind that the edges of the camera's viewport cannot be out of bounds.
    Subtract/Add half of the width by the position (much like the focus) and test it then.
*/

void Camera::placeInBounds()
{		
    //
    if((positionCoord.x) - (width / 2.0) <=  0.0)
    {
        positionCoord.x = (width / 2.0);
    }
    else if(positionCoord.x + ((width / 2.0)) >= (double)levelWidth*zoomScale)
    {
        positionCoord.x = (((double)levelWidth*zoomScale) - (width / 2.0));
    }
    
    //y
    if((positionCoord.y) - (height / 2.0) <= 0.0)
    {
        positionCoord.y = ((height / 2.0));
    }
    else if(positionCoord.y + ((height / 2.0)) >= (double)levelHeight*zoomScale)
    {
        positionCoord.y = (((double)levelHeight*zoomScale) - (height / 2.0));
    }
    
}

const bool Camera::isOutOfBoundsX(const double x) const 
{
    return (((x - (width / 2.0)) <= 0.0) || ((x + (width / 2.0)) >= (double)levelWidth));
}

const bool Camera::isOutOfBoundsY(const double y) const
{
    return (((y - (height / 2.0)) <= 0.0) || ((y + (height / 2.0)) >= (double)levelHeight));
}

/*
 The position of the camera is the top-left. 
 The users will want the focus point. This is achieved 
 by subtracting have the width of the camera/screen size
 and then returning that value.
 */

const Vector2D Camera::getFocusPoint() const
{ 
    return Vector2D(fabs(positionCoord.x - (width / 2.0)), fabs(positionCoord.y - (height / 2.0))); 
}

/*
 Return the value of zoomScale.
 This way, objects can be scaled in the drawing phase
 and achieve a camera effect.
 */

const double Camera::getZoom() const
{
    return zoomScale;
}

/*
 To place an object on screen correctly relative to the
 camera's position. The user must subract the real
 position by the camera's focused position. This gets annoying
 sometimes so here is a function that does this automatically.
 */

const Vector2D Camera::worldToScreen(Vector2D worldPos)
{
    return Vector2D((worldPos.x*zoomScale) - (getFocusPoint().x), 
                    (worldPos.y*zoomScale) - (getFocusPoint().y));
}

/*
 Using the previous equation, it is now re-written to take
 a world coordinate into a screen coordinate.
 */

const Vector2D Camera::screenToWorld(Vector2D screenPos)
{
    return Vector2D((screenPos.x + getFocusPoint().x)/zoomScale, 
                    (screenPos.y + getFocusPoint().y)/zoomScale);
}

/*
 Test against whether or not an object is
 in the camera's viewport. If it is not, 
 the object can be culled thus
 reducing objects drawn and increasing speed of game
 */

const bool Camera::isPositionInView(const Vector2D worldPos)
{
    bool toReturn = false;
    
    double tempzoomScale = zoomScale;
    
    if(zoomScale < 1) //Only check things in view when the camera is expanded
        tempzoomScale = 1;
    
    if(worldPos.x < (positionCoord.x + width)/tempzoomScale && 
        worldPos.x > (positionCoord.x - width)/tempzoomScale &&
        worldPos.y < (positionCoord.y + height)/tempzoomScale && 
        worldPos.y > (positionCoord.y - height)/tempzoomScale)
    {
        toReturn = true;
    }
    
    return toReturn;
}

void Camera::update()
{	
    if(moveToTarget)
    {
    /*For now 
     Keep Y static
     Make X move
     */
        double scaledSpeed = speedValue*zoomScale;
        double scaledTargetX, scaledTargetY;
    
        scaledTargetX = target.x*zoomScale + EPSILON;
        scaledTargetY = target.y*zoomScale + EPSILON;
        
        positionCoord.y = scaledTargetY;
        double tempX = 0.0, tempY = 0.0;

        /*if(positionCoord.x < scaledTargetX)
        {
            tempX = positionCoord.x + scaledSpeed;
            
            if(tempX >= scaledTargetX)
            {
                positionCoord.x = scaledTargetX; moveToTarget = false;
            }
            else
            {
                positionCoord.x += fabs(positionCoord.x - tempX); 
            }
        }
        else
        {
            tempX = positionCoord.x - scaledSpeed;
    
            if(tempX <= scaledTargetX)
            {
                positionCoord.x = scaledTargetX; moveToTarget = false;
            }
            else
            {
                positionCoord.x -= fabs(positionCoord.x - tempX); 
            }
        }
        
        if(positionCoord.y < scaledTargetY)
        {
            tempY = positionCoord.y + scaledSpeed;
            
            if(tempY >= scaledTargetY)
            {
                positionCoord.y = scaledTargetY; moveToTarget = false;
            }
            else
            {
                positionCoord.y += fabs(positionCoord.y - tempY); 
            }
        }
        else
        {
            tempY = positionCoord.y - scaledSpeed;
            
            if(tempY <= scaledTargetY)
            {
                positionCoord.x = scaledTargetY; moveToTarget = false;
            }
            else
            {
                positionCoord.y -= fabs(positionCoord.y - tempY); 
            }
        }*/
        
        positionCoord = Vector2D(scaledTargetX, scaledTargetY);
    }
        
    /*
     Another function to place the camera in bounds of level
     */
   placeInBounds();
}

const bool Camera::isActive() const
{
    return activeState;
}

const double Camera::getSpeed() const
{
    return speedValue;
}

void Camera::setViewportSize(const int width, const int height)
{
    this->width = width;
    this->height = height;
}

const int Camera::getViewportWidth() const
{
    return width;
}

const int Camera::getViewportHeight() const
{
    return height;
}

const Vector2D Camera::getViewportCoord() const
{
    return viewportCoord2D;
}

/*
 Set the new vieport coordinates
 */

void Camera::setViewportCoord(Vector2D newCoord)
{
    viewportCoord2D = newCoord;
}

/*
 Set the speed. This is how fast the camera will be able go when 
 following a target.
 */

void Camera::setSpeed(const double speed)
{
    double newSpeed = speed;
    if(newSpeed <= EPSILON)
    {
        newSpeed = EPSILON; //avoid divisions and such of zero!
    }
    
    this->speedValue = newSpeed;
}

/*
 Set the focus (target). The camera moves to this position by speed.
 */

void Camera::setFocus(const Vector2D newFocus)
{
    /*if(newFocus.x*zoomScale == positionCoord.x || newFocus.y*zoomScale == positionCoord.y)
        moveToTarget = false;
 
    Vector2D drawable = worldToScreen(newFocus);
    
    //x
    if(drawable.x <= 0.0)
    {	
        target.x = 0.0;
    }
    else if(drawable.x >= (double)levelWidth*zoomScale)
    {
        target.x = ((double)levelWidth*zoomScale) - (width / 2.0);
    }
    else
    {
        target.x = newFocus.x;
    }
    
    //y
    if(drawable.y <= 0.0)
    {
        target.y = 0.0;
    }
    else if(drawable.y >= (double)levelHeight*zoomScale)
    {
        target.y = ((double)levelHeight*zoomScale) - (height / 2.0);
    }
    else
    {
        target.y = newFocus.y;
    }
    
    //You have the target. Now go!*/
    target = newFocus;
    
    moveToTarget = true;
}

/*
 How much the camera should zoomScale in or out
 */

void Camera::setZoom(const double z)
{
    if(z < 1)
    {
        zoomScale = 1;
    }
    
    zoomScale = z;
    
    warpFocus(target);
}

/*
 Turn the camera on or off. 
 Multiple cameras can be turned on 
 and have their own viewports. 
 These features can be used together to
 create instant splitscreens.
 (i.e. two player games)
 */

void Camera::toggleActive(const bool state)
{
    activeState = state;
}

/*
 Warp the focus. Warps the focus to the specified position.
 Unlike the setFocus() function, the camera will not move 
 to this position, it will instantly warp to the designated spot in the level
 */

void Camera::warpFocus(const Vector2D newFocus)
{
    Vector2D drawable = worldToScreen(newFocus);
    
    //x
    if(drawable.x <= 0.0)
    {	
        positionCoord.x = 0;
        target.x = positionCoord.x;
    }
    else if(newFocus.x + ((width / 2.0)) >= (double)levelWidth*zoomScale)
    {
        positionCoord.x = ((double)levelWidth*zoomScale) - (width / 2.0);
        target.x = positionCoord.x;
    }
    else
    {
        positionCoord.x = newFocus.x*zoomScale;
        
        /*
         Any future calls to setFocus() will overthrow
         everything we did here. To prevent this,
         position.x is equal to the virtaul target X position (in cam view)
         and the target is left with the literal X position. 
         The next time update() is called, it will calculate
         that it is on the target and no glitchy camera movement 
         will occur
         */
        
        target.x = newFocus.x;
    }
    
    //y
    if(drawable.y <= 0.0)
    {
        positionCoord.y = 0;
        target.y = positionCoord.y;
    }
    else if(newFocus.y + ((height / 2.0)) >= (double)levelHeight*zoomScale)
    {
        positionCoord.y = ((double)levelHeight*zoomScale) - (height / 2.0);
        target.y = positionCoord.y;
    }
    else
    {
        positionCoord.y = newFocus.y*zoomScale;
        
        /*
         Any future calls to setFocus() will overthrow
         everything we did here. To prevent this,
         position.y is equal to the virtaul target Y position (in cam view)
         and the target is left with the literal Y position. 
         The next time update() is called, it will calculate
         that it is on the target and no glitchy camera movement 
         will occur
         */
        
        target.y = newFocus.y;
    }
    
    moveToTarget = true;
    
    update();
    
    setFocus(newFocus);
}

void Camera::resetCamera()
{
    positionCoord.x = width/2.0, positionCoord.y = height/2.0;
    velCoord.x = velCoord.y = target.x = target.y = 0.0;
    speedValue = 10.0;
    friction = 0.95;
    zoomScale = 1.0; //default
    moveToTarget = false;
}