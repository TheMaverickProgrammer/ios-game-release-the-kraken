#include "KrakenGameTiledMap.h"
    
KrakenGameTiledMapCarMode::KrakenGameTiledMapCarMode(TiledQT_TMXFileParser *parser) : KrakenGameTiledMap()
{
    map_parser = parser;

    size_t found = map_parser->getTilesetFileLocation().find_last_of("/\\");
    std::string file = map_parser->getTilesetFileLocation().substr(found+1);
    
    std::wstring w_file = Gosu::widen(file);
    
    v_img = Gosu::loadTiles(Core::get().getGraphics(),
                            Gosu::resourcePrefix() + w_file,
                            map_parser->getCellWidth(),
                            map_parser->getCellHeight(),
                            true);

    int GID = map_parser->getFirstGID();

    for(int y = 0; y < map_parser->getCellHeight() * v_img.size(); y += map_parser->getCellHeight())
    {
        for(int x = 0; x < map_parser->getCellWidth() * v_img.size(); x += map_parser->getCellWidth())
        {
            GID++;
        }
    }

    readFromParser(*map_parser);
    
    shader = new GosuEx::Shader(Core::get().getGraphics());
    
    std::wstring shader_file = Gosu::resourcePrefix() + L"pixelate.frag";
    
    fragment_program = shader->compile(shader_file);
}

KrakenGameTiledMapCarMode::~KrakenGameTiledMapCarMode() 
{

}

void KrakenGameTiledMapCarMode::draw(Camera *cam)
{
    if(cam == 0)
        return;
    
    int cell_width  = getCellWidth();
    int cell_height = getCellHeight();

    for(int k = 0; k < getNumLayers(); k++)
    {
        if(!map_parser->getLayer(k).visible)
            continue;
        
        for(int i = 0; i < getMapWidth(); i++)
        {                
            for(int j = 0; j < getMapHeight(); j++)
            {
                GenericTiledMap::Tile tile = getTile(i, j, k);
                
                if(tile.getCellID() - 1  < 0)
                    continue;
                
                if(!cam->isPositionInView(Vector2D(i*cell_width, j*cell_height)))
                    continue;
                
                Gosu::Color c = Gosu::Colors::white;
                c.setAlpha( map_parser->getLayer(k).opacity * 255 );
                
                Vector2D coord = cam->worldToScreen( Vector2D( (i*cell_width), (j*cell_height) ) );
                
                double scale = cam->getZoom();
                
                // NOW DRAW
                v_img[tile.getCellID()-1].draw(coord.x, coord.y, k, scale, scale, c);
                
                if(k == 0)
                {
                    c.setAlpha(10);
                    
                    Vector2D coord = cam->worldToScreen( 
                        Vector2D( (i*cell_width)+(cosf(Gosu::milliseconds()/200)*64),
                                  (j*cell_height)+(sinf(Gosu::milliseconds()/200)*64)));
                                                        
                    v_img[tile.getCellID()-1].draw(coord.x, coord.y, k+1, scale, scale, c, Gosu::amAdditive);

                }
            }
        }
    }
    
    std::cout << abs((int)(cosf(Gosu::milliseconds()/500.0)*9.0)) << std::endl;
    shader->uniform1i(fragment_program, (char*)"in_WindowWidth", Core::get().getScreenWidth() );
    shader->uniform1i(fragment_program, (char*)"in_WindowHeight", Core::get().getScreenHeight() );
    shader->uniform1i(fragment_program, (char*)"in_Texture", shader->internalTexture(Core::get().getGraphics()));
    shader->uniform1i(fragment_program, (char*)"in_PixelSize", abs((int)(cosf(Gosu::milliseconds()/500.0)*9.0)) );
    
    shader->run(fragment_program);
}

// ================
// PLANE MODE

KrakenGameTiledMapPlaneMode::KrakenGameTiledMapPlaneMode(TiledQT_TMXFileParser *parser) : KrakenGameTiledMap()
{
    map_parser = parser;
    
    size_t found = map_parser->getTilesetFileLocation().find_last_of("/\\");
    std::string file = map_parser->getTilesetFileLocation().substr(found+1);
    
    std::wstring w_file = Gosu::widen(file);
    
    v_img = Gosu::loadTiles(Core::get().getGraphics(),
                            Gosu::resourcePrefix() + w_file,
                            map_parser->getCellWidth(),
                            map_parser->getCellHeight(),
                            true);
    
    int GID = map_parser->getFirstGID();
    
    for(int y = 0; y < map_parser->getCellHeight() * v_img.size(); y += map_parser->getCellHeight())
    {
        for(int x = 0; x < map_parser->getCellWidth() * v_img.size(); x += map_parser->getCellWidth())
        {
            GID++;
        }
    }
    
    readFromParser(*map_parser);
}

KrakenGameTiledMapPlaneMode::~KrakenGameTiledMapPlaneMode() 
{
    
}

void KrakenGameTiledMapPlaneMode::draw(Camera *cam)
{
    if(cam == 0)
        return;
    
    int cell_width  = getCellWidth();
    int cell_height = getCellHeight();
    
    Core::get().getGraphicResources().get("PlaneBackground")->draw(0,0,0);
    
    for(int k = 0; k < getNumLayers(); k++)
    {
        if(!map_parser->getLayer(k).visible)
            continue;
        
        for(int i = 0; i < getMapWidth(); i++)
        {                
            for(int j = 0; j < getMapHeight(); j++)
            {
                GenericTiledMap::Tile tile = getTile(i, j, k);
                
                if(tile.getCellID() - 1  < 0)
                    continue;
                
                if(!cam->isPositionInView(Vector2D(i*cell_width, j*cell_height)) &&
                   !cam->isPositionInView(Vector2D((i+1)*cell_width, j*cell_height)) &&
                   !cam->isPositionInView(Vector2D(i*cell_width, (j+1)*cell_height)))
                    continue;
                
                Gosu::Color c = Gosu::Colors::white;
                c.setAlpha( map_parser->getLayer(k).opacity * 255 );
                
                Vector2D coord = cam->worldToScreen( Vector2D( (i*cell_width), (j*cell_height) ) );
                
                double scale = cam->getZoom();
                
                // NOW DRAW
                if(k == 3) // ON CLOUD LAYER
                {
                }
                
                v_img[tile.getCellID()-1].draw(coord.x, coord.y, k, scale, scale, c);
                
                if(k == 0)
                {
                    c.setAlpha(10);
                    
                    Vector2D coord = cam->worldToScreen( 
                                                        Vector2D( (i*cell_width)+(cosf(Gosu::milliseconds()/200)*64),
                                                                 (j*cell_height)+(sinf(Gosu::milliseconds()/200)*64)));
                    
                    v_img[tile.getCellID()-1].draw(coord.x, coord.y, k+1, scale, scale, c, Gosu::amAdditive);
                    
                }
            }
        }
    }
}