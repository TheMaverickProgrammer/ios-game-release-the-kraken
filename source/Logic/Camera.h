/*
 *  Camera.h
 *
 *  Created by The Maverick Programmer on 7/19/09.
 *  Copyright 2009 The Maverick Programmer. All rights reserved.
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2D.h"

class Camera
{
private:
    
    double speedValue, friction, zoomScale;
    Vector2D positionCoord, velCoord, target;
    int width, height, levelWidth, levelHeight;
    bool moveToTarget;
    bool activeState;
    
    Vector2D viewportCoord2D;
    
    const bool isOutOfBoundsX(const double) const;
    const bool isOutOfBoundsY(const double) const;

    void placeInBounds();
public:
    
    const double getZoom() const;
    const int getViewportWidth() const;
    const int getViewportHeight() const;
    const Vector2D getViewportCoord() const;
    
    const Vector2D getFocusPoint() const;
    const double getSpeed() const;
    const Vector2D worldToScreen(Vector2D);
    const Vector2D screenToWorld(Vector2D);
    const bool isPositionInView(Vector2D);
    const bool isActive() const;
    
    void setWorldDimensions(const int, const int);
    void setSpeed(const double);
    void setViewportSize(const int, const int);
    void setZoom(const double);
    void setViewportCoord(Vector2D);
    void toggleActive(bool);
    
    Vector2D getFocus() { return target; }
    
    void setFocus(Vector2D);
    void warpFocus(Vector2D);

    void update();
    void resetCamera();
    
    Camera() 
    { 
        width = 800; //Default values until initialized
        height = 600;
        viewportCoord2D.zero();
        levelWidth = width;
        levelHeight = height; // Until camera knows a level, it is only size of viewport
        positionCoord.x = 0;
        positionCoord.y = 0;
        velCoord.x = velCoord.y = target.x = target.y = 0.0;
        speedValue = 10.0;
        friction = 0.95;
        zoomScale = 1.0; // Default scale value.
        moveToTarget = false;
        activeState = true; // Default. Camera is on.
        
        update(); // Force the first update
    }
    
    ~Camera(){;}
};	
#endif
