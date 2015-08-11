#include "KrakenGameState.h"

KrakenGameState::KrakenGameState(SELECTED_GAME_OPTION& sgo, GAME_PLAYER_TYPE &gpt)
: LogicState()
{
    
    is_paused = false;
    pause_counter = 0;
    
    current_state = DEFAULT;
    
    // PARSE MAP AND INFO
    TiledQT_TMXFileParser *parser = new TiledQT_TMXFileParser();
    
    switch(gpt)
    {
        case CAR_TYPE:
            switch(sgo)
            {
                case SUPER_SQUID:
                    parser->parseTMXFile(Gosu::narrow(Gosu::resourcePrefix() + L"/squid.tmx"));
                    break;
                case KILLER_KRAKEN:
                    parser->parseTMXFile(Gosu::narrow(Gosu::resourcePrefix() + L"/kraken.tmx"));
                    break;
                case CRAZY_CTHULHU:
                    parser->parseTMXFile(Gosu::narrow(Gosu::resourcePrefix() + L"/cthulhu.tmx"));
                    break;
            }
            break;
            
        case PLANE_TYPE:
            switch(sgo)
            {
                case SUPER_SQUID:
                    parser->parseTMXFile(Gosu::narrow(Gosu::resourcePrefix() + L"/squid_cloud.tmx"));
                    break;
                case KILLER_KRAKEN:
                    parser->parseTMXFile(Gosu::narrow(Gosu::resourcePrefix() + L"/kraken_cloud.tmx"));
                    break;
                case CRAZY_CTHULHU:
                    parser->parseTMXFile(Gosu::narrow(Gosu::resourcePrefix() + L"/cthulhu_cloud.tmx"));
                    break;
            }
            break;
    }
    
    // MAKE MAP FROM PARSER
    if(gpt != PLANE_TYPE)
        map = new KrakenGameTiledMapCarMode(parser);
    else
        map = new KrakenGameTiledMapPlaneMode(parser);
    
    // INITIATE GROUPS    
    entity_group = new EntityGroup();
    
    // CREATE CAMERA
    camera = new Camera();
    camera->setWorldDimensions(parser->getMapWidth()*parser->getCellWidth(), 
                               parser->getMapHeight()*parser->getCellHeight());
    
    camera->setViewportSize(Core::get().getScreenWidth(), Core::get().getScreenHeight());
    
    // Create the player
    p = 0;
    
    switch(gpt)
    {
        case CAR_TYPE:
            p = new CarPlayer( 8*9, 8*11 );
            break;
        
        case PLANE_TYPE:
            p = new PlanePlayer(8*9, 8*11);
            break;
    }
    
    p->noticeMap(map);
    p->noticeCamera(camera);
    
    entity_group->add(p);
    
    // CREATE PREDATOR
    predator = 0;
    
    if(gpt != PLANE_TYPE)
    {
        switch(sgo) 
        {
            case SUPER_SQUID:
                predator = new SquidEasy();
                break;
            case KILLER_KRAKEN:
                predator = new KrakenNormal();
                break;
            default:
                predator = 0;
        }
            
        
        predator->noticeMap(map);
        predator->noticeCamera(camera);
        predator->noticePlayer(p);
        
        entity_group->add(predator);
    }
    
    
    // Different modes require different cash values. Example: Needed cash = $40,000 by 50s = 800 x 50
    int cash_value = 1;
    
    switch(sgo)
    {
        case SUPER_SQUID:
            cash_value = 300; // $15,000
            break;
        case KILLER_KRAKEN:
            cash_value = 600; // $30,000
            break;
        case CRAZY_CTHULHU:
            cash_value = 1000; // $50,000
            break;
    }
    
    gui = new PlayerGUI(cash_value);
    gui->noticePlayer(p);
    gui->noticeMap(map);
    gui->noticePredator(predator);
    
    //entity_group->add(gui);
    
    game_mode = sgo;
    player_type = gpt;
    
    // ZOOM IN
    if(gpt != PLANE_TYPE)
        camera->setZoom(4);
    else
        camera->setZoom(16);
    
    // Play song!
    if(player_type == PLANE_TYPE)
    {
        playSong(BONUS);
    }
    else
    {
        switch(game_mode)
        {
            case SUPER_SQUID:
                playSong(EASY);
                break;
            case KILLER_KRAKEN:
                playSong(NORMAL);
                break;
            case CRAZY_CTHULHU:
                playSong(HARD);
                break;
        }
    }
}

KrakenGameState::~KrakenGameState()
{
}

void KrakenGameState::onUpdate(LogicStateMachine *logic)
{
    static bool paused_press = false;
    
    if(is_paused)
    {
        pause_counter++;
        
        if(pause_counter > 2250)
            pause_counter = 0;
        
        if(Core::get().getInput().down(Gosu::kbEscape))
        {
            if(player_type == PLANE_TYPE)
            {
                logic->popLogicState();
                logic->popLogicState();
            }
            else
            {
                logic->popLogicState();
            }
        }
        
        if(Core::get().getInput().down(Gosu::kbP))
        {
            if(!paused_press)
            {
                is_paused = false;
                paused_press = true;
                
                if(predator)
                     predator->resetClock();
            }
        }
        else
            paused_press = false;
        
        return;
    }
    else
    {
        if(Core::get().getInput().down(Gosu::kbP))
        {
            if(!paused_press)
            {
                is_paused = true;
                paused_press = true;
            }
        }
        else
            paused_press = false;
    }
    
    // Returning to this state from another state!
    if(current_state != DEFAULT)
    {
        KrakenMenuState::SaveFileInfo save_file_info;
        save_file_info.loadFile();
        
        this->setCollectedMoney(save_file_info.getTempVariable(), false);
        
        current_state = DEFAULT;
        
        // Play song!
        if(player_type == PLANE_TYPE)
        {
            playSong(BONUS);
        }
        else
        {
            switch(game_mode)
            {
                case SUPER_SQUID:
                    playSong(EASY);
                    break;
                case KILLER_KRAKEN:
                    playSong(NORMAL);
                    break;
                case CRAZY_CTHULHU:
                    playSong(HARD);
                    break;
            }
        }
    }
    
    if((int)camera->getZoom() == 4)
    {
        entity_group->update();
    }
    
    camera->update();
    gui->onUpdate();
    
    if(p->getSpeed() >= 0.8)
    {
        entity_group->add(
                          new SpeedBlur(
                                        Core::get().getGraphicResources().get("Player"), 
                                        camera, 
                                        Vector2D(p->getX(), p->getY()), 
                                        (255.0*p->getSpeed())/1.5, 
                                        20)
                          );
    }
    
    if(p->isGameOver())
    {
        logic->popLogicState();
        
        // Prompt to try again
        logic->pushLogicState(new TryAgainState(game_mode));
        
        stopAllSongs();
        
    }
    else if(p->hasCompleteMap())
    {
        // Check if ACCOMPLISHED or just COMPLETE
        
        // Save results to file
        KrakenMenuState::SaveFileInfo save_file_info;
        save_file_info.loadFile();
        
        if(gui->getTotalCash() >= gui->getCashNeeded() && player_type != PLANE_TYPE)
        {
            save_file_info.setAccomplishedToID((int)game_mode);
            save_file_info.saveFile();
        }
        else if(player_type == PLANE_TYPE)
        {
            save_file_info.setTempVariable(gui->getTotalCash());
            save_file_info.saveFile();
        }
    
        stopAllSongs();
        
        // see if they won the lease if CAR MODE
        if(player_type == CAR_TYPE)
        {
            bool try_again = false; 
            
            if(gui->getTotalCash() < gui->getCashNeeded()) // Not enough, try again
                try_again = true;
            
            logic->popLogicState(); // leave this mode
            logic->pushLogicState(new CheckLease(game_mode, try_again));
            
            stopAllSongs();
        }
        else // It was bonus stage, so just pop back
        {
            logic->popLogicState();
        }
    }
    
    CarPlayer *car = dynamic_cast<CarPlayer*>(p);
    
    if(car != 0)
    {        
        if(car->gotoPlaneMinigame())
        {
            stopAllSongs();
            
            if(camera->getZoom() >= 16)
            {
                GAME_PLAYER_TYPE plane_mode_only = PLANE_TYPE; 
                
                //KrakenGameState *NextGameState = new KrakenGameState(game_mode, plane_mode_only);
                
                //NextGameState->setCollectedMoney(gui->getTotalCash());
                
                //logic->pushLogicState(new KrakenGameState(game_mode, plane_mode_only));
                
                KrakenGameState *NextGameState = new KrakenGameState(game_mode, plane_mode_only);
                NextGameState->setCollectedMoney(gui->getTotalCash(), true);
                
                LogicState *type_cast_state = NextGameState;
                
                logic->pushLogicState(type_cast_state);
                
                car->unflagPlaneMinigame();
                
                current_state = RETURNING; // One minus prev state. Return state is 1. No action at 0.
               
                //delete NextGameState;
                //NextGameState = 0;
                
                
            }
            else
            {
                camera->setZoom(camera->getZoom()+0.2);
            }
        }
        else
        {
            if(camera->getZoom() > 4)
            {
                camera->setZoom(camera->getZoom()-0.1);
                
                // Reset predator clock (affects AI)
                // Keep doing so until camera stops (will promise AI is fair)
                if(predator) // If predator exists
                {
                    predator->resetClock();
                }
            }
        }
    }
    else
    {
        if(camera->getZoom() > 4)
        {
            camera->setZoom(camera->getZoom()-0.1);
        }
    }
    
    if(camera->getZoom() < 4)
        camera->setZoom(4);
}

void KrakenGameState::onDraw()
{        
    map->draw(camera);

    entity_group->draw();
    
    // DO WEATHER EFFECTS
    if(player_type == CAR_TYPE)
    {
        if(game_mode == KILLER_KRAKEN)
        {
            if(Gosu::random(0, 100) >= 98)
            {
                // Lightning flash
                Gosu::Color flash = Gosu::Colors::white;
                flash.setAlpha(Gosu::random(10, 25));
                Core::get().getGraphics().drawQuad(0, 0, flash, 
                                                   Core::get().getScreenWidth(), 0, flash, 
                                                   Core::get().getScreenWidth(), Core::get().getScreenHeight(), flash,
                                                   0, Core::get().getScreenHeight(), flash, 
                                                   10, Gosu::amAdditive);
            }
            else
            {
                // Darken screen
                Gosu::Color flash = Gosu::Colors::black;
                flash.setAlpha(25);
                Core::get().getGraphics().drawQuad(0, 0, flash, 
                                                   Core::get().getScreenWidth(), 0, flash, 
                                                   Core::get().getScreenWidth(), Core::get().getScreenHeight(), flash,
                                                   0, Core::get().getScreenHeight(), flash, 
                                                   10);
            }
            
            for(int i = 0; i < 100; i++)
            {
                double rand_value_x = Gosu::random(0, Core::get().getScreenWidth());
                double rand_value_y = Gosu::random(0, Core::get().getScreenHeight());
                
                Core::get().getGraphics().drawLine(rand_value_x, rand_value_y, Gosu::Colors::blue, rand_value_x-10, rand_value_y+20, Gosu::Colors::blue, 10);
                
                Gosu::Color c = Gosu::Colors::white;
                c.setAlpha(255/2.0);
                
                Core::get().getGraphics().drawLine(rand_value_x-1, rand_value_y+1, c, rand_value_x-11, rand_value_y+21, c, 10, Gosu::amAdditive);
            }
        }
    }
    
    gui->onDraw();
    
    if(player_type == CAR_TYPE)
    {
        CarPlayer *car = dynamic_cast<CarPlayer*>(p);
        
        if(car != 0)
        {     
            if(!is_paused) // It would overlap in drawing if we're paused
            {
                switch(car->getBalloonCount())
                {
                    case 1:
                        gui->writeNote("Spelling: 'F__'");
                        break;
                    case 3:
                        gui->writeNote("Spelling: 'FL_'");
                        break;
                    case 7:
                        gui->writeNote("Spelling: 'FLY'");
                        break;
                    case 2:
                        gui->writeNote("Spelling: '_L_'");
                        break;
                    case 6:
                        gui->writeNote("Spelling: '_LY'");
                        break;
                    case 4:
                        gui->writeNote("Spelling: '__Y'");
                        break;
                    case 5:
                        gui->writeNote("Spelling: 'F_Y'");
                        break;
                }
            }
        }
    }
    else // PLANE_TYPE
    {
        if(!is_paused)
            gui->writeNote("BONUS ROUND!");
    }
    
    // Paused?
    
    if(is_paused)
    {        
        // Draw the white square that will be used to fade in and out:
        Gosu::Color fade = Gosu::Colors::black;
        fade.setAlpha(0.5*255.0);
        Core::get().getGraphics().drawQuad(0, 0, fade, 
                                           Core::get().getScreenWidth(), 0, fade, 
                                           Core::get().getScreenWidth(), Core::get().getScreenHeight(), fade,
                                           0, Core::get().getScreenHeight(), fade, 
                                           11);
        
        if(pause_counter < 250)
            gui->writeNote("PAUSED");
        else if(pause_counter < 500)
            gui->writeNote("ESC. RETURNS TO MENU");
        else if(pause_counter < 750)
            gui->writeNote("PAUSED");
        else if(pause_counter < 1000)
            gui->writeNote("GAME MADE BY BRIAN PEPPERS");
        else if(pause_counter < 1250)
            gui->writeNote("PAUSED");
        else if(pause_counter < 1500)
            gui->writeNote("ESC. RETURNS TO MENU");
        else if(pause_counter < 1750)
            gui->writeNote("PAUSED");
        else if(pause_counter < 2000)
            gui->writeNote("GAME MADE BY BRIAN PEPPERS");
        else if(pause_counter < 2250)
            gui->writeNote("YOU'RE BORED, AREN'T YOU?");
    }
}