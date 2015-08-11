#include "Core.h"
#include "Kraken/KrakenGame.h"
#include <vector>
#include <iostream>
#include <exception>

class KrakenGameApp : public Gosu::Window
{
private:
    LogicStateMachine *GameLogic;    
    
public:
    
    KrakenGameApp() // SET USING GLOBAL CORE PARAMETERS
    :   Gosu::Window(Core::get().getScreenWidth(), Core::get().getScreenHeight(), Core::get().isFullscreen())
    {
        // SETTING CAPTION
        setCaption(Core::get().getAppName());
        
        // SET GLOBAL CORE DEVICES
        Core::get().setGraphics(graphics());
        Core::get().setInput(input());
        
        // INITIALIZE GAME LOGIC
        GameLogic = new KrakenGame();
    }
    
    void update()
    {
        // PRESS "ESC" TO QUIT
       // if(input().down(Gosu::kbEscape))
         //   close();
        
        GameLogic->update();
        
        if(GameLogic->isEmpty())
            close();
    }
    
    void draw()
    {
        // use renderer (i.e. camera) to draw graphics
        GameLogic->draw();
    }
};


namespace Gosu
{
    extern bool undocumentedRetrofication;
}

int main()
{
    try 
    {
        Gosu::undocumentedRetrofication = true; 
        
        Core::createSettings(600, 400, L"Re-Lease the Kraken!", false);
        KrakenGameApp window;
        window.show();
    }
    catch(std::exception &e)
    {
        std::cout << "Here's the error message: " << e.what() << std::endl;
    }
}