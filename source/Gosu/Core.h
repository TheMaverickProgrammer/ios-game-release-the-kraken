#ifndef CORE_H
#define CORE_H

#include "Entity.h"

#include "Dependencies.h"
#include "Resources.h"

class Core
{
private:
     Gosu::Graphics* graphics_device;
     Gosu::Input   * input_device;
     int screen_width;
     int screen_height;
     std::wstring app_name;
     bool fullscreen;
 
     static Core *gosu_core;
 
     // Add some resources for graphics and sound
    Resources<Gosu::Image*> graphic_resources;
    Resources<Gosu::Sample*> soundfx_resources;
    Resources<Gosu::Song*>  song_resources;
public:
     
 static void createSettings(int screen_w, int screen_h, std::wstring name, bool is_fullscreen)
 {
     if(gosu_core)
         return;
     
     gosu_core = new Core(screen_w, screen_h, name, is_fullscreen);
 }
    
 Core(int screen_w, int screen_h, std::wstring name, bool is_fullscreen)
 {
     graphics_device = 0;
     input_device    = 0;
     screen_width    = screen_w;
     screen_height   = screen_h;
     app_name        = name;
     fullscreen      = is_fullscreen;
     gosu_core       = this;
 }
 
 Resources<Gosu::Image*> &getGraphicResources() { return graphic_resources; }
  
 Resources<Gosu::Sample*> &getSoundfxResources() { return soundfx_resources; }
    
 Resources<Gosu::Song*> &getSongResources() { return song_resources; }
    
 void setGraphics(Gosu::Graphics &graphics) { graphics_device = &graphics; }
    
 void setInput(Gosu::Input &input) { input_device = &input; }
    
 static Core &get() { return *gosu_core; } 
    
 void release() { ; }
 
 const bool isFullscreen() { return fullscreen; }
 
 const std::wstring getAppName() { return app_name; }
 
 const int getScreenWidth()      { return screen_width; }
 
 const int getScreenHeight()     { return screen_height; }
 
 Gosu::Input &getInput()         { return *input_device; }
 
 Gosu::Graphics &getGraphics()   { return *graphics_device; }
};

#endif