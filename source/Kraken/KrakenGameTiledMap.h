#ifndef Kraken_GAME_TILED_MAP
#define Kraken_GAME_TILED_MAP

#include "../Logic/Map/TiledQT/TiledQT_TiledMap.h"
#include "../Logic/Camera.h"

#include <map>
#include <algorithm>
#include <functional>

#include "../Gosu/Core.h"
#include "../Gosu/GosuEx_Shader.h"

class KrakenGameTiledMap : public TiledQT_TiledMap
{
public:
    KrakenGameTiledMap() : TiledQT_TiledMap()
    { ; }
    
    ~KrakenGameTiledMap() { ; }
    
    virtual void draw(Camera *cam) = 0;
    
    GosuEx::Shader *shader;
    GosuEx::ShaderProgram fragment_program;
};

class KrakenGameTiledMapCarMode : public KrakenGameTiledMap
{
private:
    TiledQT_TMXFileParser *map_parser;

    std::vector<Gosu::Image> v_img;
    
public:
    
    KrakenGameTiledMapCarMode(TiledQT_TMXFileParser *parser);
    
    ~KrakenGameTiledMapCarMode();
    
    void draw(Camera *cam);
};

class KrakenGameTiledMapPlaneMode : public KrakenGameTiledMap
{
private:
    TiledQT_TMXFileParser *map_parser;
    
    std::vector<Gosu::Image> v_img;
    
public:
    
    KrakenGameTiledMapPlaneMode(TiledQT_TMXFileParser *parser);
    
    ~KrakenGameTiledMapPlaneMode();
    
    void draw(Camera *cam);
};
#endif