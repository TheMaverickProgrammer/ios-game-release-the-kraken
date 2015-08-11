#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "../Logic/LogicState.h"
#include "KrakenGameTiledMap.h"
#include "Player.h"
#include "KrakenNormal.h"
#include "SquidEasy.h"
#include "Camera.h"
#include "EntityGroup.h"
#include "Sequence.h"

// Game state stuff

enum SELECTED_GAME_OPTION { SUPER_SQUID, KILLER_KRAKEN, CRAZY_CTHULHU };
enum GAME_PLAYER_TYPE     { CAR_TYPE,    PLANE_TYPE };
enum GAME_TYPE_TRANSITION { DEFAULT,     CHANGING,      RETURNING     };

class KrakenGameState : public LogicState
{
private:
    Camera               *camera;
    KrakenGameTiledMap   *map;
    
    EntityGroup    *entity_group;
    
    Player         *p;
    
    PlayerGUI      *gui;
    
    Predator       *predator;
    
    SELECTED_GAME_OPTION game_mode;
    GAME_PLAYER_TYPE     player_type;
    
    GAME_TYPE_TRANSITION current_state;
    
    bool is_paused;
    int pause_counter;
    
public:
    KrakenGameState(SELECTED_GAME_OPTION& sgo, GAME_PLAYER_TYPE &gpt);
    ~KrakenGameState();
    
    void onUpdate(LogicStateMachine *logic);
    void onDraw();
    
    void setCollectedMoney(int money, bool lock)
    {
        if(lock)
            gui->setLockAmount(money);
        else
            gui->changeCashTo(money);
    }
    
    const int getTotalCashFromGui()
    {
        return gui->getTotalCash(); 
    }
};

class KrakenGameOverState : public LogicState
{
public:
    KrakenGameOverState() { ;}
    ~KrakenGameOverState() { ; }
    
    void onUpdate(LogicStateMachine *logic)
    {
        
    }
    
    void onDraw()
    {
        Core::get().getGraphicResources().get("GameOver")->draw(0, 0, 0);
    }
};

/*******************************************
** KrakenMenuState
** Has 10 images that make up the menu. These images have a position on the screen and move to spots.
********************************************/

class KrakenMenuState : public LogicState
{
public:
    
    class MenuImage : public Entity // Moves to a spot from a spot given an amount of time
    {
    private: 
        Gosu::Image *toDraw;
        Vector2D    position;
        Vector2D    target;
        double      speed;
        bool        arrived;
        
    public:
        MenuImage(Gosu::Image* img, Vector2D start_pos, Vector2D end_pos, double speed)
        {
            toDraw      = img;
            position    = start_pos;
            target      = end_pos;
            this->speed = speed;
            arrived     = false;
        }
        
        const bool hasArrived() const { return arrived; }
        
        void update()
        {
            arrived = interpolateToTarget(position, target, speed);
        }
        
        void setNewTarget(Vector2D targetPos)
        {
            target = targetPos;
        }
        
        bool interpolateToTarget(Vector2D &pos, Vector2D targetPos, const double speed)
        {            
            if( pos.x != targetPos.x || pos.y != targetPos.y )
            {
                bool isGreaterX = false, isGreaterY = false;
                
                if(targetPos.x < pos.x)
                    isGreaterX = true;
                
                if(targetPos.y < pos.y)
                    isGreaterY = true;
                
                Vector2D dir = Vector2D( targetPos.x - pos.x , targetPos.y - pos.y );
                
                double speedScalar = speed / sqrtf ( (dir.x * dir.x) + (dir.y * dir.y) );
                
                Vector2D newPos = Vector2D( pos.x, pos.y );
                
                newPos.x += dir.x * speedScalar;
                newPos.y += dir.y * speedScalar;
                
                //Check X
                if(newPos.x > targetPos.x && !isGreaterX)
                {
                    pos.x = targetPos.x;
                }
                else if(newPos.x < targetPos.x && isGreaterX)
                {
                    pos.x = targetPos.x;
                }
                else
                    pos.x = newPos.x;
                
                //Check Y
                if(newPos.y > targetPos.y && !isGreaterY)
                {
                    pos.y = targetPos.y;
                }
                else if(newPos.y < targetPos.y && isGreaterY)
                {
                    pos.y = targetPos.y;
                }
                else
                    pos.y = newPos.y;
                
                /*//Check X
                 if (  fabs( targetPos.x - newPos.x )+EPSILON > abs( dir.x ) )
                 {
                 pos.x = targetPos.x;
                 }
                 else
                 {
                 pos.x = newPos.x;
                 }
                 
                 //Check Y
                 if (  fabs( targetPos.y - newPos.y )+EPSILON >  abs( dir.y ) )
                 {
                 pos.y = targetPos.y;
                 }
                 else
                 {
                 pos.y = newPos.y;
                 }*/
                
                // pos = newPos;
                
                return false;
                
            }
            
            return true;
        }
        
        void draw()
        {
            if(toDraw)
                toDraw->draw(position.x, position.y, 10);
        }
        
        ~MenuImage() { ; }
    };
    
private:
    MenuImage *arrow, *background, *boardwalk, *car, *car_shadow, *easy, *hard, *kraken, *normal, *title_name, *help;
    double white_alpha;
    bool show_title, show_bg, show_checkmarks;
    int mode_selection, final_mode; // 1 Easy | 2 Normal | 3 Hard
    Sequence menu_state_sequence;
    
public:
    struct SaveFileInfo
    {
        bool accomplished[3];
        
        int  highest_money[3];
        
        int temp_space;
        
        bool first_time;
        
        SaveFileInfo()
        {
            accomplished[0] = false;
            accomplished[1] = false;
            accomplished[2] = false;
            
            highest_money[0] = 0;
            highest_money[1] = 0;
            highest_money[2] = 0;
            
            temp_space       = 0;
            
            first_time = true; 
        }
        
        const bool hasAccomplishedByID(int ID)
        {
            return accomplished[ID];
        }
        
        void setAccomplishedToID(int ID)
        {
            accomplished[ID] = true;
        }
        
        const bool getMoneyByID(int ID)
        {
            return highest_money[ID];
        }
        
        void setMoneyByID(int ID)
        {
            highest_money[ID] = true;
        }
        
        void setTempVariable(int value)
        {
            temp_space = value;
        }
        
        void setFirstTime(bool value)
        {
            first_time = value;
        }
        
        const bool isFirstTime() 
        {
            return first_time; 
        }
        
        const int getTempVariable()
        {
            return temp_space;
        }
        
        void loadFile()
        {
            std::ifstream file;
            file.open(Gosu::narrow(Gosu::resourcePrefix() + L"0.0").c_str());
            std::string buff;
            
            int i = 0; 
            
            // First three elements is the accomplished!
            while(!file.eof())
            {
                if(i > 2)
                    break;
                
                std::getline(file, buff);
                
                if(buff == "1")
                    accomplished[i] = true;
                
                i++;
            }
            
            i = 0;
            
            // Second three elements are top money!
            while(!file.eof())
            {
                if(i > 2)
                    break;
                
                std::getline(file, buff);
                
                highest_money[i] = boost::lexical_cast<int>(buff);
                
                i++;
            }
            
            // second to last element is temp data
           // while(!file.eof())
            {
                std::getline(file, buff);
                
                temp_space = boost::lexical_cast<int>(buff);
            }
            
            // Last element is first time playing flag
            // while(!file.eof())
            {
                std::getline(file, buff);
                
                first_time = boost::lexical_cast<bool>(buff);
            }
            
            file.close();
        }
        
        void saveFile()
        {
            std::ofstream file;
            file.open(Gosu::narrow(Gosu::resourcePrefix() + L"0.0").c_str());
            
            int i = 0;
            
            // First three-set is accomplished data
            while(i < 3)
            {
                file << accomplished[i] << std::endl;
                i++;
            }
            
            i = 0;
            
            // Second three-set is highest money count
            while(i < 3)
            {
                file << highest_money[i] << std::endl;
                i++;
            }
            
            // Second to last variable is just temp data
            file << temp_space << std::endl;
            
            // last variable is first time playing
            file << first_time << std::endl;
            
            file.close();
        }
    } save_file_info;
    
    
public:
    KrakenMenuState() 
    {
        arrow      = new MenuImage(Core::get().getGraphicResources().get("Arrow"), Vector2D(-100, 0), Vector2D(), 5);
        background = new MenuImage(Core::get().getGraphicResources().get("Background"), Vector2D(), Vector2D(), 1);
        boardwalk  = new MenuImage(Core::get().getGraphicResources().get("Boardwalk"), Vector2D(400,0), Vector2D(), 9);
        car        = new MenuImage(Core::get().getGraphicResources().get("Car"), Vector2D(800,0), Vector2D(), 9);
        car_shadow = new MenuImage(Core::get().getGraphicResources().get("Car Shadow"), Vector2D(800,0), Vector2D(), 9);
        easy       = new MenuImage(Core::get().getGraphicResources().get("Easy"), Vector2D(-800,0), Vector2D(), 9);
        hard       = new MenuImage(Core::get().getGraphicResources().get("Hard"), Vector2D(-800,0), Vector2D(), 9);
        kraken     = new MenuImage(Core::get().getGraphicResources().get("Kraken Image"), Vector2D(-800,0), Vector2D(), 9);
        normal     = new MenuImage(Core::get().getGraphicResources().get("Normal"), Vector2D(800,0), Vector2D(), 9);
        title_name = new MenuImage(Core::get().getGraphicResources().get("Title Name"), Vector2D(0,150), Vector2D(), 5);
        help       = new MenuImage(Core::get().getGraphicResources().get("FirstTimeHelp"), Vector2D(0,1500), Vector2D(), 15); 
        
        
        mode_selection  = 1;
        final_mode      = -1;
        white_alpha     = 0;
        show_title      = false; 
        show_bg         = false;
        show_checkmarks = false;
        
        // open file
        save_file_info.loadFile();
        
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::slideInBackgroundObjects, this));
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::flashWhite, this));
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::fadeOut, this));
        
        if(save_file_info.isFirstTime())
        {
            menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::slideInHelp, this));
            menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::waitForSpace, this));
            menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::slideOut, this));
        }
                                                
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::waitForAnyKey, this));
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::moveAllMenuItemsToScreen, this));
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::moveInArrow, this));
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::listenForMenuInput, this));
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::moveEverythingElseAway, this));
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::fadeOutWhite, this));
        menu_state_sequence.addEventFunction(boost::bind(&KrakenMenuState::gotoSelectedMode, this));
        
        // Play music
        playSong(TITLE);
    }
    
    ~KrakenMenuState() { stopAllSongs(); }
    
    bool slideInHelp()
    {
        help->update();
        
        if(help->hasArrived())
            return true;
        else
            return false;
    }

    bool waitForSpace()
    {
        if(Core::get().getInput().down(Gosu::kbSpace))
            return true;
        else
            return false;
    }

    bool slideOut() 
    {
        help->setNewTarget(Vector2D(0, 1500));
        
        help->update();
        
        if(help->hasArrived())
        {
            save_file_info.setFirstTime(false);
            save_file_info.saveFile();
            return true;
        }
        else
            return false;
    }
    
    bool slideInBackgroundObjects()
    {
        car->update();
        car_shadow->update();
        boardwalk->update();
        kraken->update();
        
        if(car->hasArrived() && car_shadow->hasArrived() &&
           boardwalk->hasArrived() && kraken->hasArrived())
            return true;
        else
            return false;
    }
    
    bool flashWhite()
    {
        white_alpha = 255;
        return true;
    }
    
    bool fadeOut()
    {
        show_bg = true;
        
        if(white_alpha > 0.0)
            white_alpha -= 5.0;
        else
        {
            white_alpha = 0.0;
            return true;
        }
        
        return false;
    }
    
    bool waitForAnyKey()
    {
        show_title = true;
        
        if(Core::get().getInput().down(Gosu::kbSpace))
        {
            show_title = true;
            
            return true;
        }
        else
        {
            static int delay = 0;
            
            if((++delay)%100 == 99)
            {
                show_title = !show_title;
                delay = 0;
            }
        }
        
        return false;
    }
    
    bool moveAllMenuItemsToScreen()
    {
        title_name->update();
        easy->update();
        normal->update();
        hard->update();
        
        show_checkmarks = true;
        
        if(title_name->hasArrived() && easy->hasArrived() &&
           normal->hasArrived() && hard->hasArrived())
            return true;
        else
            return false;
    }
    
    bool moveAllMenuItemsToScreen2()
    {
        playSong(TITLE);
        
        // Save file probably changed during last game state. Check upon returning to menu.
        save_file_info.loadFile();
        
        show_checkmarks = true;
        
        title_name->update();
        easy->update();
        normal->update();
        hard->update();
        kraken->update();
        car->update();
        car_shadow->update();
        boardwalk->update();
        
        
        if(title_name->hasArrived() && easy->hasArrived() &&
           normal->hasArrived() && hard->hasArrived() &&
           kraken->hasArrived() && car_shadow->hasArrived() && 
           car->hasArrived()    && boardwalk->hasArrived())
            return true;
        else
            return false;
    }
    
    bool moveInArrow()
    {
        arrow->update();
        
        if(arrow->hasArrived())
            return true;
        else
            return false;
    }
    
    bool listenForMenuInput()
    {
        static bool has_pressed = false;
        
        if(Core::get().getInput().down(Gosu::kbEscape))
        {
            mode_selection = 4;
            return true;
        }
        
        // The player can move up and down the menu and press enter
        if(Core::get().getInput().down(Gosu::kbUp))
        {
            if(!has_pressed)
            {
                has_pressed = true;
                mode_selection--;
                playSoundFX(PICKUP_100);
            }
        }
        else if(Core::get().getInput().down(Gosu::kbDown))
        {
            if(!has_pressed)
            {
                has_pressed = true;
                mode_selection++;
                playSoundFX(PICKUP_50);
            }
        }
        else
        {
            has_pressed = false;
        }
        
        if(mode_selection>3) mode_selection = 3;
        if(mode_selection<1) mode_selection = 1;
        
        switch(mode_selection)
        {
            case 1:
                arrow->setNewTarget(Vector2D(0, 0));
                break;
            case 2:
                arrow->setNewTarget(Vector2D(0, 52));
                break;
            case 3:
                arrow->setNewTarget(Vector2D(0, 104));
                break;
        }
        
        arrow->update();
        
        // Now listen for the player to press enter 
        // BUT- the arrow has to no longer be moving
        
        if(Core::get().getInput().down(Gosu::kbSpace) && arrow->hasArrived())
        {
            playSoundFX(BOOST);
            return true; // We have a selection!
        }
        
        return false;
    }
    
    bool moveEverythingElseAway() // except for the selected mode
    {
        show_checkmarks = false;
        
        title_name->setNewTarget(Vector2D(0, -500));
        kraken->setNewTarget(Vector2D(-800,0));
        car->setNewTarget(Vector2D(800,0));
        car_shadow->setNewTarget(Vector2D(800,0));
        boardwalk->setNewTarget(Vector2D(800,0));
        
        switch (mode_selection) 
        {
            case 1:
                normal->setNewTarget(Vector2D(800,0));
                hard->setNewTarget(Vector2D(-800,0));
                break;
            case 2:
                easy->setNewTarget(Vector2D(-800,0));
                hard->setNewTarget(Vector2D(-800,0));
                break;
            case 3:
                easy->setNewTarget(Vector2D(-800,0));
                normal->setNewTarget(Vector2D(800,0));
                break;
            case 4: //quitting...
                easy->setNewTarget(Vector2D(-800,0));
                normal->setNewTarget(Vector2D(800,0));
                hard->setNewTarget(Vector2D(-800,0));
                break;
        }
        
        title_name->update();
        kraken->update();
        car->update();
        car_shadow->update();
        boardwalk->update();
        
        easy->update();
        normal->update();
        hard->update();
        
        //arrow->setNewTarget(Vector2D(-100,0));
        
        if(title_name->hasArrived() && easy->hasArrived() && normal->hasArrived()
               && hard->hasArrived() && arrow->hasArrived() && kraken->hasArrived()
               && car->hasArrived() && car_shadow->hasArrived() && boardwalk->hasArrived() )
            return true;
        else
            return false;
    }
    
    bool fadeOutWhite()
    {
        if(white_alpha < 255)
            white_alpha += 5.0;
        
        if(white_alpha >= 255)
        {
            white_alpha = 255;
            return true;
        }
        
        return false;
    }
    
    bool gotoSelectedMode()
    {
        final_mode = mode_selection;
        
        // We won't be here anymore anyways
        return true; // loop back?
    }
    
    void onUpdate(LogicStateMachine *logic)
    {
        menu_state_sequence.update();
        
        if(final_mode != -1) // We've decided...
        {
            SELECTED_GAME_OPTION mode; 
            GAME_PLAYER_TYPE     player_type; // WILL ALWAYS BE CAR FROM MENU
                                              // BUT DEBUG MAY TEST OTHER TYPES 
            
            player_type = CAR_TYPE; // if debug, PLANE_TYPE, else, CAR_TYPE!!!!
            
            switch (final_mode) 
            {
                case 1:
                    mode = SUPER_SQUID;
                    logic->pushLogicState(new KrakenGameState(mode, player_type));
                    break;
                case 2:
                    mode = KILLER_KRAKEN;
                    logic->pushLogicState(new KrakenGameState(mode, player_type));
                    break;
                case 3:
                    // logic->pushLogicState();
                    break;
                case 4:
                    logic->popLogicState(); //quit
                    break;
            }
            
            final_mode = -1; 
            
            // Save file probably changed during last game state. Check again.
            save_file_info.loadFile();
            
            // Reset everything to a completed transition
            //arrow      = new MenuImage(Core::get().getGraphicResources().get("Arrow"), Vector2D(-100, 0), Vector2D(), 5);
            //background = new MenuImage(Core::get().getGraphicResources().get("Background"), Vector2D(), Vector2D(), 1);
            boardwalk->setNewTarget(Vector2D());
            car->setNewTarget(Vector2D());
            car_shadow->setNewTarget(Vector2D());
            easy->setNewTarget(Vector2D());
            hard->setNewTarget(Vector2D());
            kraken->setNewTarget(Vector2D());
            normal->setNewTarget(Vector2D());
            title_name->setNewTarget(Vector2D());
            
            
            // We want to create a new sequence ONCE only after
            // The full intro is shown upon loading the game
            // Afterwards, only the options sliding back onto the screen
            // should be important.
            // Once this new sequence is assigned, it would be pointless
            // to continue the assignment again. 
            static bool once_transfer = false; 
            
            if(!once_transfer)
            {
                Sequence new_menu_sequence;
                new_menu_sequence.addEventFunction(boost::bind(&KrakenMenuState::fadeOut, this));
                new_menu_sequence.addEventFunction(boost::bind(&KrakenMenuState::moveAllMenuItemsToScreen2, this));
                new_menu_sequence.addEventFunction(boost::bind(&KrakenMenuState::moveInArrow, this));
                new_menu_sequence.addEventFunction(boost::bind(&KrakenMenuState::listenForMenuInput, this));
                new_menu_sequence.addEventFunction(boost::bind(&KrakenMenuState::moveEverythingElseAway, this));
                new_menu_sequence.addEventFunction(boost::bind(&KrakenMenuState::fadeOutWhite, this));
                new_menu_sequence.addEventFunction(boost::bind(&KrakenMenuState::gotoSelectedMode, this));
                
                menu_state_sequence = new_menu_sequence;
                
                once_transfer = true;
            }
        }
    }
    
    void onDraw()
    {
        if(show_bg)
            background->draw();
        
        boardwalk->draw();
        car->draw();
        car_shadow->draw();
        kraken->draw();
        normal->draw();
        easy->draw();
        hard->draw();
        arrow->draw();
        
        if(show_title)
            title_name->draw();
        
        if(show_checkmarks)
        {
            if(easy->hasArrived() && save_file_info.hasAccomplishedByID(0))
                Core::get().getGraphicResources().get("Checkmark")->draw(500, 120, 11);
            
            if(normal->hasArrived() && save_file_info.hasAccomplishedByID(1))
                Core::get().getGraphicResources().get("Checkmark")->draw(540, 172, 11);
            
            if(hard->hasArrived() && save_file_info.hasAccomplishedByID(2))
                Core::get().getGraphicResources().get("Checkmark")->draw(480, 240, 11);
        }
        
        help->draw();
        
        // Draw the white square that will be used to fade in and out:
        Gosu::Color fade = Gosu::Colors::white;
        fade.setAlpha(white_alpha);
        Core::get().getGraphics().drawQuad(0, 0, fade, 
                                            Core::get().getScreenWidth(), 0, fade, 
                                            Core::get().getScreenWidth(), Core::get().getScreenHeight(), fade,
                                            0, Core::get().getScreenHeight(), fade, 
                                            11, Gosu::amAdditive);
    }
};

// Yes -> go back 1 state
// No  -> go back to menu 
class TryAgainState : public LogicState
{
private:
    KrakenMenuState::MenuImage *TryAgain;
    KrakenMenuState::MenuImage *YesNo;
    KrakenMenuState::MenuImage *arrow;
    
    int selected, final;
    
    double white_alpha;
    
    SELECTED_GAME_OPTION game_mode;
    
    Sequence sequence;
public:
    
    TryAgainState(SELECTED_GAME_OPTION prev_game_mode) : LogicState() 
    {
        game_mode = prev_game_mode;
        
        TryAgain = new KrakenMenuState::MenuImage
             (Core::get().getGraphicResources().get("TryAgain"), Vector2D(-140, 50), Vector2D(140, 50), 5);
        
        YesNo    = new KrakenMenuState::MenuImage
             (Core::get().getGraphicResources().get("YesNo"), Vector2D(-900, 135), Vector2D(248, 135), 10);
        
        arrow = new KrakenMenuState::MenuImage
             (Core::get().getGraphicResources().get("Arrow"), Vector2D(-677, 0), Vector2D(177, 0), 5);
    
        sequence.addEventFunction(boost::bind(&TryAgainState::moveInToScreen, this));
        sequence.addEventFunction(boost::bind(&TryAgainState::listenForMenuInput, this));
        sequence.addEventFunction(boost::bind(&TryAgainState::fadeOutWhite, this));
        
        selected = 1;
        final    = -1;
        
        white_alpha = 0;
    }
    
    bool moveInToScreen()
    {
        TryAgain->update();
        YesNo->update();
        arrow->update();
        
        if(TryAgain->hasArrived() && YesNo->hasArrived() && arrow->hasArrived())
            return true;
        else
            return false;
    }
    
    bool listenForMenuInput()
    {
        static bool has_pressed = false;
        
        // The player can move up and down the menu and press enter
        if(Core::get().getInput().down(Gosu::kbUp))
        {
            if(!has_pressed)
            {
                has_pressed = true;
                selected--;
                playSoundFX(PICKUP_100);
            }
        }
        else if(Core::get().getInput().down(Gosu::kbDown))
        {
            if(!has_pressed)
            {
                has_pressed = true;
                selected++;
                playSoundFX(PICKUP_50);
            }
        }
        else
        {
            has_pressed = false;
        }
        
        if(selected>2) selected = 2;
        if(selected<1) selected = 1;
        
        switch(selected)
        {
            case 1:
                arrow->setNewTarget(Vector2D(177, 0));
                break;
            case 2:
                arrow->setNewTarget(Vector2D(177, 100));
                break;
        }
        
        arrow->update();
        
        // Now listen for the player to press enter 
        // BUT- the arrow has to no longer be moving
        
        if(Core::get().getInput().down(Gosu::kbSpace) && arrow->hasArrived())
        {
            playSoundFX(BOOST);
            return true; // We have a selection!
        }
        
        return false;
    }
    
    bool fadeOutWhite()
    {
        if(white_alpha < 255)
            white_alpha += 5.0;
        
        if(white_alpha >= 255)
        {
            white_alpha = 255;
            
            final = selected;
            return true;
        }
        
        return false;
    }
    
    void onUpdate(LogicStateMachine *logic)
    {
        sequence.update();
        
        if(final != -1)
        {
            if(final == 1)
            {
                logic->popLogicState();
                GAME_PLAYER_TYPE type = CAR_TYPE;
                
                logic->pushLogicState(new KrakenGameState(game_mode, type));
            }
            else
            {
                logic->popLogicState();
            }
        }
    }
    
    void onDraw()
    {
        arrow->draw();
        YesNo->draw();
        TryAgain->draw();
        
        // Draw the white square that will be used to fade in and out:
        Gosu::Color fade = Gosu::Colors::white;
        fade.setAlpha(white_alpha);
        Core::get().getGraphics().drawQuad(0, 0, fade, 
            Core::get().getScreenWidth(), 0, fade, 
            Core::get().getScreenWidth(), Core::get().getScreenHeight(), fade,
            0, Core::get().getScreenHeight(), fade, 
            11, Gosu::amAdditive);
    }
};

// May be approved, may not.
// Also different images for the modes.

// Lease moves in from bottom of screen
// Approved slides in if money is good, 
   //fades out white 
       //and goes to menu
// Failure slides in if bad
   // fades out black
      //goes to TryAgain

class CheckLease : public LogicState
{
private:
    bool try_again;
    double color_alpha;
    
    bool go_to_next_scene;
    
    SELECTED_GAME_OPTION game_mode;
    KrakenMenuState::MenuImage *lease_image;
    KrakenMenuState::MenuImage *sign;
    
    Sequence sequence;
    
public:
    CheckLease(SELECTED_GAME_OPTION mode, bool try_again)
    {
        color_alpha = 0.0;
        
        
        game_mode = mode;
        this->try_again = try_again;
        
        go_to_next_scene = false;
        
        switch(game_mode)
        {
            case SUPER_SQUID:
                    lease_image = new KrakenMenuState::MenuImage
                    (Core::get().getGraphicResources().get("SquidLease"), Vector2D(-300, 0), Vector2D(), 5);
                break;
            case KILLER_KRAKEN:
                    lease_image = new KrakenMenuState::MenuImage
                    (Core::get().getGraphicResources().get("KrakenLease"), Vector2D(-300, 0), Vector2D(), 5);
                break;
            case CRAZY_CTHULHU:
                    lease_image = new KrakenMenuState::MenuImage
                    (Core::get().getGraphicResources().get("CthulhuLease"), Vector2D(-300, 0), Vector2D(), 5);
                break;
        }
        
        if(try_again)
        {
            // Failure
            sign = new KrakenMenuState::MenuImage
            (Core::get().getGraphicResources().get("Failed"), Vector2D(-800, 0), Vector2D(), 5);
        }
        else
        {
            // Approved
            sign = new KrakenMenuState::MenuImage
            (Core::get().getGraphicResources().get("LeaseApproved"), Vector2D(-900, 0), Vector2D(), 5);
        }
        
        sequence.addEventFunction(boost::bind(&CheckLease::slideInLease, this));
        sequence.addEventFunction(boost::bind(&CheckLease::slideInSign, this));
        sequence.addEventFunction(boost::bind(&CheckLease::fadeOut, this));
    }
    
    ~CheckLease() { ; }
    
    bool slideInLease()
    {
        lease_image->update();
        
        if(lease_image->hasArrived())
        {
            return true;
        }
        
        return false;
    }
    
    bool slideInSign()
    {
        sign->update();
        
        if(sign->hasArrived())
        {
            return true;
        }
        
        return false;
    }
    
    bool fadeOut()
    {
        color_alpha += 5.0;
        
        if(color_alpha >= 255.0)
        {
            color_alpha = 255.0;
            go_to_next_scene = true;
            return true;
        }
        
        return false;
    }
    
    void onUpdate(LogicStateMachine *logic)
    {
        sequence.update();
        
        if(go_to_next_scene)
        {
            if(try_again)
            {
                logic->popLogicState();
                logic->pushLogicState(new TryAgainState(game_mode));
            }
            else
            {
                logic->popLogicState();
            }
        }
    }
    
    void onDraw()
    {
        lease_image->draw();
        sign->draw();
        
        // Draw fade out
        if(try_again)
        {
            Gosu::Color fade = Gosu::Colors::black;
            fade.setAlpha(color_alpha);
            Core::get().getGraphics().drawQuad(0, 0, fade, 
                                               Core::get().getScreenWidth(), 0, fade, 
                                               Core::get().getScreenWidth(), Core::get().getScreenHeight(), fade,
                                               0, Core::get().getScreenHeight(), fade, 
                                               11);
        }
        else
        {
            Gosu::Color fade = Gosu::Colors::white;
            fade.setAlpha(color_alpha);
            Core::get().getGraphics().drawQuad(0, 0, fade, 
                                               Core::get().getScreenWidth(), 0, fade, 
                                               Core::get().getScreenWidth(), Core::get().getScreenHeight(), fade,
                                               0, Core::get().getScreenHeight(), fade, 
                                               11, Gosu::amAdditive);
        }
    }
};


class LoadResourcesGameState : public LogicState
{
private:
    bool shownLoading;
    
public:
    
    LoadResourcesGameState() : LogicState() { shownLoading = false;; }
    ~LoadResourcesGameState() { ; }
    
    Gosu::Graphics &graphics()
    {
        return Core::get().getGraphics();    
    }
    
    void loadResources()
    {
        if(!shownLoading)
            return;
        
        // Player - CAR
        Core::get().getGraphicResources().bind("Player", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 5*8, 2*8, 16, 16, false ) );
        
        Core::get().getGraphicResources().bind("Player2", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 9*8, 2*8, 16, 16, false ) );
        
        // Player - PLANE
        Core::get().getGraphicResources().bind("PlanePlayer",
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 2*16, 0, 16, 16, false ) );
        
        Core::get().getGraphicResources().bind("PlanePlayer2",
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 3*16, 0, 16, 16, false ) );
        
        // Plane mode BACKGROUND
        Core::get().getGraphicResources().bind("PlaneBackground",
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"land_back.png", false ) );
        
        // Kraken - NORMAL
        Core::get().getGraphicResources().bind("KrakenBody", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 3*8, 6*8, 8*8, 8*6, false ) );
        
        // Kraken - NORMAL ARM PIECES
        Core::get().getGraphicResources().bind("KrakenArm", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 5*8, 13*8, 8, 8, false ) );
        Core::get().getGraphicResources().bind("KrakenEnd", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 6*8, 13*8, 8, 8, false ) );
        
        // Squid PIECES *********
        Core::get().getGraphicResources().bind("SquidBody",
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 7*8, 12*8, 32, 24, false) );
        // ARM PIECES
        Core::get().getGraphicResources().bind("SquidArm", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 5*8, 14*8, 8, 8, false ) );
        Core::get().getGraphicResources().bind("SquidEnd", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 6*8, 14*8, 8, 8, false ) );
        
        // Tile - Grey
        Core::get().getGraphicResources().bind("Grey", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 4*16, 2*16, 32, 16, false ) );
        
        // Tile - Blue
        Core::get().getGraphicResources().bind("Blue", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 2*16, 2*16, 32, 16, false ) );
        
        // Tile - Money
        Core::get().getGraphicResources().bind("Money", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 6*16, 2*16, 16, 16, false ) );
        
        // Particle
        Core::get().getGraphicResources().bind("Particle", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken.png", 4*16, 0*16, 16, 16, false ) );
        
        // GameOver
        Core::get().getGraphicResources().bind("GameOver", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"gameover.png", false ) );
        
        // Menu
        Core::get().getGraphicResources().bind("Menu", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"menu.png", false ) );
        
        // TRY AGAIN GRAPHICS
        Core::get().getGraphicResources().bind("TryAgain", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"Try_again.png", false ) );
        
        // Yes / No
        Core::get().getGraphicResources().bind("YesNo", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"yes_no.png", false ) );
        
        // GAME OVER BACKGROUND
        Core::get().getGraphicResources().bind("GameOverBackground", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"bg_game_over.png", false ) );
        
        // Failed
        Core::get().getGraphicResources().bind("Failed", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"Failed.png", false ) );
        
        // LEASE GRAPHICS
        
        // Squid Lease
        Core::get().getGraphicResources().bind("SquidLease", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"Lease_Squid.png", false ) );
        
        // Kraken Lease
        Core::get().getGraphicResources().bind("KrakenLease", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"Lease_Kraken.png", false ) );
        
        // Cthulhu Lease
        Core::get().getGraphicResources().bind("CthulhuLease", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"Lease_Cthulhu.png", false ) );
        
        // Lease Approved
        Core::get().getGraphicResources().bind("LeaseApproved", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"lease_approved.png", false ) );
        
        // TITLE SCREEN STUFF ***********
        
        // Arrow
        Core::get().getGraphicResources().bind("Arrow", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"arrow.png", false ) );
        
        // Background
        Core::get().getGraphicResources().bind("Background", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"bg.png", false ) );
        
        // Boardwalk
        Core::get().getGraphicResources().bind("Boardwalk", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"board_walk.png", false ) );
        
        // Car
        Core::get().getGraphicResources().bind("Car", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"car.png", false ) );
        
        // Car Shadow
        Core::get().getGraphicResources().bind("Car Shadow", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"car_shadow.png", false ) );
        
        // Easy
        Core::get().getGraphicResources().bind("Easy", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"easy.png", false ) );
        
        // Normal
        Core::get().getGraphicResources().bind("Normal", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"normal.png", false ) );
        
        // Hard
        Core::get().getGraphicResources().bind("Hard", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"hard.png", false ) );
        
        // Kraken
        Core::get().getGraphicResources().bind("Kraken Image", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"kraken_image.png", false ) );
        
        // Title
        Core::get().getGraphicResources().bind("Title Name", 
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"title.png", false ) );
        
        // NEW -- CHECKMARK
        Core::get().getGraphicResources().bind("Checkmark",
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"check.png", false) );
        
        // NEW -- First Time Help
        Core::get().getGraphicResources().bind("FirstTimeHelp",
                                               new Gosu::Image( graphics(),
                                                               Gosu::resourcePrefix() + L"first_time_help.png", false) );       
        
        // AUDIO **************
        Core::get().getSoundfxResources().bind("Bonus_Sweep", new Gosu::Sample(Gosu::resourcePrefix() + L"Bonus_Sweep.wav"));
        
        Core::get().getSoundfxResources().bind("Destroyed", new Gosu::Sample(Gosu::resourcePrefix() + L"Destroyed.wav"));
        
        Core::get().getSoundfxResources().bind("Pickup50", new Gosu::Sample(Gosu::resourcePrefix() + L"Money50_Pickup.wav"));
        
        Core::get().getSoundfxResources().bind("Pickup100", new Gosu::Sample(Gosu::resourcePrefix() + L"Money100_Pickup.wav"));
        
        Core::get().getSoundfxResources().bind("Boost", new Gosu::Sample(Gosu::resourcePrefix() + L"Speed_Boost.wav"));
        
        Core::get().getSoundfxResources().bind("Slowdown", new Gosu::Sample(Gosu::resourcePrefix() + L"Speed_Slowdown.wav"));
        
        // MUSIC **************
        
        // Title
        Core::get().getSongResources().bind("Tune", new Gosu::Song(Gosu::resourcePrefix() + L"Familiar_Tune.ogg"));
        
        // Easy mode
        Core::get().getSongResources().bind("Easy", new Gosu::Song(Gosu::resourcePrefix() + L"Dashing_Daring.ogg"));
        
        // Normal mode
        Core::get().getSongResources().bind("Normal", new Gosu::Song(Gosu::resourcePrefix() + L"Fast_Lane.ogg"));
        
        // Hard mode
        Core::get().getSongResources().bind("Hard", new Gosu::Song(Gosu::resourcePrefix() + L"Grudge.ogg"));
        
        // Bonus music
        Core::get().getSongResources().bind("Bonus", new Gosu::Song(Gosu::resourcePrefix() + L"Airborne_Freedom.ogg"));
        
    }
    
    void onUpdate(LogicStateMachine *logic)
    {
        loadResources();
        
        if(shownLoading)
        {
            logic->popLogicState();
            logic->pushLogicState(new KrakenMenuState());
        }
    }
    
    void onDraw()
    {
        static Gosu::Image* loading = new Gosu::Image(graphics(), Gosu::resourcePrefix() + L"loading.png", false);
        
        loading->draw(50, Core::get().getScreenHeight()/2.0, 1);
        
        shownLoading = true;
    }
};

#endif