#include "Player.h"

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

// PLAYER CLASS BEGIN
Player::Player(int x, int y)
    : smoke_emitter(), boost_emitter(), money_emitter(), Entity()
{
    position = Vector2D(x, y);
    
    row = 2; // There are only three rows
    
    // speed
    speed = 0.5;
    
    // was on a boost or not
    did_boost = false;
    
    game_over = false;
    
    complete_map = false;
    
    boost_emitter.setImage(Core::get().getGraphicResources().get("Particle"));
    boost_emitter.setLayer(5);
    boost_emitter.setLifeRange(10, 40);
	boost_emitter.setAlphaRange(50, 100);
    boost_emitter.setScaleRange(0.1, 2.0);
    boost_emitter.setDirection(0.0);
    boost_emitter.setSpread(360);
    boost_emitter.setMax(200);
    boost_emitter.velocity() = Vector2D(4.0, 4.0);
    
    money_emitter.setImage(Core::get().getGraphicResources().get("Money"));
    money_emitter.setLayer(5);
    money_emitter.setLifeRange(10, 40);
	money_emitter.setAlphaRange(50, 100);
    money_emitter.setScaleRange(0.1, 0.5);
    boost_emitter.setMax(200);
    money_emitter.velocity() = Vector2D(4.0, 4.0);
    money_emitter.setSpread(360);
    
    collected_money = 0;
}

Player::~Player()
{
}

const bool Player::isPointInside(Vector2D point)
{
    if(point.x >= position.x-8 
       && point.x <= position.x+8
       && point.y >= position.y-8 
       && point.y <= position.y+8 )
        return true;
    
    return false;
}

const int Player::getX()
{
    return position.x;
}

const int Player::getY()
{
    return position.y;
}

void Player::noticeMap(KrakenGameTiledMap *map)
{
    this->map = map;
}

void Player::noticeCamera(Camera* cam)
{
    this->camera = cam;
    this->camera->warpFocus(Vector2D(getX(), getY()));
}


void Player::pollInput()
{
    static bool pressed = false;
    
    if(Core::get().getInput().down(Gosu::kbUp))
    {
        if(pressed)
            return;
        
        row--;
        pressed = true;
    }
    else if(Core::get().getInput().down(Gosu::kbDown))
    {
        if(pressed)
            return;
        
        row++;
        pressed = true;
    }
    else
    {
        pressed = false;
    }
    
    if(row < 1) row = 1;
    if(row > 3) row = 3;
}

const double Player::getSpeed()
{
    return speed;    
}

void Player::emitBoostParticles()
{
    for(int i = 0; i < 36; i++)
    {
        boost_emitter.add();
    }
}

void Player::emitMoneyParticles()
{
    for(int i = 0; i < 18; i++)
    {
        money_emitter.add();
    }
}

// PLAYER CLASS END

// ===============

// CARPLAYER CLASS BEGIN
CarPlayer::CarPlayer(int x, int y)
: Player(x, y)
{
    FLY_COUNT = 0;
}

CarPlayer::~CarPlayer()
{
}

void CarPlayer::onDraw()
{
    money_emitter.draw(camera);
    boost_emitter.draw(camera);
    
    Vector2D coord = camera->worldToScreen(Vector2D((int)position.x-16, 4*16));
    double   scale = camera->getZoom();
    
    Core::get().getGraphicResources().get("Grey")->draw(coord.x, coord.y, 4, scale, scale);
    
    coord = camera->worldToScreen(Vector2D((int)position.x-16, 5*16));
    Core::get().getGraphicResources().get("Grey")->draw(coord.x, coord.y, 4, scale, scale);
    
    coord = camera->worldToScreen(Vector2D((int)position.x-16, 6*16));
    Core::get().getGraphicResources().get("Grey")->draw(coord.x, coord.y, 4, scale, scale);
    
    coord = camera->worldToScreen(Vector2D(position.x-16, (int)(position.y/16)*16));
    
    Core::get().getGraphicResources().get("Blue")->draw(coord.x, coord.y, 5, scale, scale);
    
    coord = camera->worldToScreen(Vector2D(position.x-8, position.y-8));
    
    if(Gosu::milliseconds()%400 <= 200)
    {
        if(Core::get().getGraphicResources().contains("Player"))
        {
            Core::get().getGraphicResources().get("Player")->draw(coord.x, coord.y, 5, scale, scale);
        }
    }
    else
    {
        if(Core::get().getGraphicResources().contains("Player2"))
        {
            Core::get().getGraphicResources().get("Player2")->draw(coord.x, coord.y, 5, scale, scale);
        }
    }
}

void CarPlayer::onUpdate()
{
    pollInput();
    
    if(row == 1 && position.y != 4.5*16)
    {
        if(position.y > 4.5*16)
            position.y -= 2; // CHANGE TO SPEED
        else
            position.y += 2;
    }
    
    if(row == 2 && position.y != 5.5*16)
    {
        if(position.y > 5.5*16)
            position.y -= 2; // CHANGE TO SPEED
        else
            position.y += 2;
    }
    
    if(row == 3 && position.y != 6.5*16)
    {
        if(position.y > 6.5*16)
            position.y -= 2; // CHANGE TO SPEED
        else
            position.y += 2;
    }
    
    position.x += speed; // Speed * 2?
    
    /** MAKE SURE THE CAMERA FOLLOWS **/
    camera->setFocus(position);
    
    // 2 is SPECIAL
    // Get the literal row position; not the cars
    int row_pos = 4.5 + ((row-1)*1.0);
    int column_pos = (position.x+8)/16;
    
    GenericTiledMap::Tile tile = map->getTile(column_pos, row_pos, 2);
    
    // #23 = Money
    // #15 = Speed
    // #47 = Ramps -> now are rocks
    
    // #7  = Slow
    // #8  = LessMoney
    // #24 = Cloud
    
    // #55 = 'F'
    // #56 = 'L'
    // #64 = 'Y'
    
    bool emit_boost = false;
    bool emit_money = false;
    
    switch(tile.getCellID())
    {
        case 23:
            map->setTile(column_pos, row_pos, 0, 2);
            emit_money = true;
            collected_money+=2;
            playSoundFX(PICKUP_100);
            break; 
        case 15:
            did_boost = true;
            speed+=1.0;
            map->setTile(column_pos, row_pos, 0, 2);
            emit_boost = true;
            playSoundFX(BOOST);
            break;
        case 47:
            did_boost = false;
            speed=0.0;
            map->setTile(column_pos, row_pos, 0, 2);
            playSoundFX(DESTROYED);
            break;
        case 55:
            FLY_COUNT+=1;
            map->setTile(column_pos, row_pos, 0, 2);
            playSoundFX(TO_BONUS);
            break;
        case 56:
            FLY_COUNT+=2;
            map->setTile(column_pos, row_pos, 0, 2);
            playSoundFX(TO_BONUS);
            break;
        case 64:
            FLY_COUNT+=4;
            map->setTile(column_pos, row_pos, 0, 2);
            playSoundFX(TO_BONUS);
            break;
    }
    

    if(!did_boost)
    {
        if(speed <= 0.5)
            speed += 0.01;
    }
    else
    {
        speed -= 0.005;
        
        if(speed <= 0.8)
        {
            speed = 0.8;
        }
    }
    
    if(emit_boost)
        emitBoostParticles();
    
    if(emit_money)
        emitMoneyParticles();
    
    if(speed >= 1.5)
        speed = 1.5;
    
    // CHECK IF WON 
    // Player has won if gone off map (to the right) x > map.width
    if(position.x > map->getMapWidth()*map->getCellWidth())
    {
        // ACCOMPLISHED --> IF Money collected is >= money needed
        // else
        // COMLPETE
        
        complete_map = true;
    }
    
    boost_emitter.position() = Vector2D(position.x-4, position.y-16);
    money_emitter.position() = Vector2D(position.x, position.y-8);
    
    money_emitter.update();
    boost_emitter.update();
    
}

// END CARPLAYER

// =============

// PLANEPLAYER CLASS BEGIN

PlanePlayer::PlanePlayer(int x, int y)
: Player(x, y)
{
    ;
}

PlanePlayer::~PlanePlayer()
{
}

void PlanePlayer::onDraw()
{
    money_emitter.draw(camera);
    boost_emitter.draw(camera);
    
    Vector2D coord = camera->worldToScreen(Vector2D((int)position.x-16, 4*16));
    double   scale = camera->getZoom();
    
    Core::get().getGraphicResources().get("Grey")->draw(coord.x, coord.y, 4, scale, scale);
    
    coord = camera->worldToScreen(Vector2D((int)position.x-16, 5*16));
    Core::get().getGraphicResources().get("Grey")->draw(coord.x, coord.y, 4, scale, scale);
    
    coord = camera->worldToScreen(Vector2D((int)position.x-16, 6*16));
    Core::get().getGraphicResources().get("Grey")->draw(coord.x, coord.y, 4, scale, scale);
    
    coord = camera->worldToScreen(Vector2D(position.x-16, (int)(position.y/16)*16));
    
    Core::get().getGraphicResources().get("Blue")->draw(coord.x, coord.y, 5, scale, scale);
    
    coord = camera->worldToScreen(Vector2D(position.x-8, position.y-8));
    
    if(Gosu::milliseconds()%100 <= 50)
    {
        if(Core::get().getGraphicResources().contains("PlanePlayer"))
        {
            Core::get().getGraphicResources().get("PlanePlayer")->draw(coord.x, coord.y, 5, scale, scale);
        }
    }
    else
    {
        if(Core::get().getGraphicResources().contains("PlanePlayer2"))
        {
            Core::get().getGraphicResources().get("PlanePlayer2")->draw(coord.x, coord.y, 5, scale, scale);
        }
    }
}

void PlanePlayer::onUpdate()
{
    pollInput();
    
    if(row == 1 && position.y != 4.5*16)
    {
        if(position.y > 4.5*16)
            position.y -= 2; // CHANGE TO SPEED
        else
            position.y += 2;
    }
    
    if(row == 2 && position.y != 5.5*16)
    {
        if(position.y > 5.5*16)
            position.y -= 2; // CHANGE TO SPEED
        else
            position.y += 2;
    }
    
    if(row == 3 && position.y != 6.5*16)
    {
        if(position.y > 6.5*16)
            position.y -= 2; // CHANGE TO SPEED
        else
            position.y += 2;
    }
    
    position.x += speed; // Speed * 2?
    
    /** MAKE SURE THE CAMERA FOLLOWS **/
    camera->setFocus(position);
    
    // 2 is SPECIAL
    // Get the literal row position; not the cars
    int row_pos = 4.5 + ((row-1)*1.0);
    int column_pos = (position.x+8)/16;
    
    GenericTiledMap::Tile tile = map->getTile(column_pos, row_pos, 2);
    
    // #23 = Money
    // #15 = Speed
    // #47 = Ramps -> now are rocks
    
    // #7  = Slow
    // #8  = LessMoney
    // #24 = Cloud
    
    // #55 = 'F'
    // #56 = 'L'
    // #64 = 'Y'
    
    bool emit_boost = false;
    bool emit_money = false;
    
    switch(tile.getCellID())
    {
        case 8:
            map->setTile(column_pos, row_pos, 0, 2);
            emit_money = true;
            collected_money++;
            playSoundFX(PICKUP_50);
            break; 
        case 7:
            //did_boost = true;
            speed=0.0;
            map->setTile(column_pos, row_pos, 0, 2);
            emit_boost = true;
            playSoundFX(SLOWDOWN);
            break;
        case 24:
            //did_boost = false;
            speed=0.0;
            map->setTile(column_pos, row_pos, 0, 2);
            collected_money = 0;
            playSoundFX(DESTROYED);
            break;
            
    }
    
    speed += 0.005;
    
    if(speed <= 0.8)
    {
        speed = 0.8;
    }

    if(emit_boost)
        emitBoostParticles();
    
    if(emit_money)
        emitMoneyParticles();
    
    if(speed >= 1.75)
        speed = 1.75;
    
    // CHECK IF WON 
    // Player has won if gone off map (to the right) x > map.width
    if(position.x > map->getMapWidth()*map->getCellWidth())
    {
        // ACCOMPLISHED --> IF Money collected is >= money needed
        // else
        // COMLPETE
        
        complete_map = true;
    }
    
    boost_emitter.position() = Vector2D(position.x-4, position.y-16);
    money_emitter.position() = Vector2D(position.x, position.y-8);
    
    money_emitter.update();
    boost_emitter.update();
    
}