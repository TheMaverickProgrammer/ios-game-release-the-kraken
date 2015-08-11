#include "GenericTiledMap.h"

// TILES

GenericTiledMap::Tile::Tile() 
{ 
    cell_x = cell_y = cell_id = -1;
}

GenericTiledMap::Tile::Tile(const int cell_x, const int cell_y, const int cell_id)
{
    this->cell_x  = cell_x;
    this->cell_y  = cell_y;
    this->cell_id = cell_id;
}

GenericTiledMap::Tile::Tile(const Tile &other)
{
    this->cell_x  = other.cell_x;
    this->cell_y  = other.cell_y;
    this->cell_id = other.cell_id;
}

void GenericTiledMap::Tile::setCellID(int cell_id)
{
    this->cell_id = cell_id;
}

const int GenericTiledMap::Tile::getCellID() const { return cell_id; }

const int GenericTiledMap::Tile::getCellX()
{
    return cell_x;    
}

const int GenericTiledMap::Tile::getCellY()
{
    return cell_y;
}

GenericTiledMap::Tile::~Tile() { ; }

// LAYERS

GenericTiledMap::Layer::Layer(const int map_width, const int map_height) 
{ 
    opacity = 0.0f; 
    visible = true; 
    v_tiles.clear(); 
    resetDimensions(map_width, map_height); 
}

GenericTiledMap::Layer::~Layer() { ; }

void GenericTiledMap::Layer::setName(std::string name) { this->name = name; }

const std::string &GenericTiledMap::Layer::getName() { return name; }

void GenericTiledMap::Layer::resetDimensions(const int map_width, const int map_height)
{
    for(int i = 0; i < map_height; i++)
    {
        v_tiles.push_back( std::vector<GenericTiledMap::Tile>() );
        
        for(int j = 0; j < map_width; j++)
        {
            v_tiles[i].push_back( Tile(j, i, 0) );
        }
    }
}

const bool GenericTiledMap::Layer::isCellFree(const int cell_x, const int cell_y) const
{
    if(cell_y < 0 || cell_y >= v_tiles.size())
        return false;
    
    if(cell_x < 0 || cell_x >= v_tiles[0].size())
        return false;
    
    return (v_tiles[cell_y][cell_x].getCellID() == 0);
}

void GenericTiledMap::Layer::setTile(const int cell_x, const int cell_y, int cell_id)
{
    if(v_tiles.empty() )
        return;
    
    if(cell_y < 0 || cell_y >= v_tiles.size())
        return;
    
    if(cell_x < 0 || cell_x >= v_tiles[0].size())
        return;
    
    v_tiles[cell_y][cell_x].setCellID( cell_id );
}

const GenericTiledMap::Tile &GenericTiledMap::Layer::getTile(const int cell_x, const int cell_y)
{
    if(cell_y < 0 || cell_y >= v_tiles.size())
        return Tile(cell_x, cell_y, 0);
    
    if(cell_x < 0 || cell_x >= v_tiles[0].size())
        return Tile(cell_x, cell_y, 0);
    
    return (v_tiles[cell_y][cell_x]);
}

// MAP

GenericTiledMap::GenericTiledMap()
{
    map_width   = 640/16;
    map_height  = 480/16;
    cell_width  = 16;
    cell_height = 16;
    num_layers  = 0;
}

GenericTiledMap::GenericTiledMap(const int map_width, const int map_height, 
            const int cell_width, const int cell_height, const int num_layers)
{ 
    setAttributes(map_width, map_height, cell_width, cell_height, num_layers);
}


GenericTiledMap::~GenericTiledMap() { ; }

const int GenericTiledMap::getMapWidth()   
{ 
    return map_width; 
}

const int GenericTiledMap::getMapHeight()  
{ 
    return map_height; 
}

const int GenericTiledMap::getNumLayers()  
{ 
    return (int)v_layers.size();
}

const int GenericTiledMap::getCellWidth()  
{ 
    return cell_width;
}

const int GenericTiledMap::getCellHeight() 
{ 
    return cell_height; 
}

void GenericTiledMap::setAttributes(const int map_width, const int map_height, 
                                    const int cell_width, const int cell_height, const int num_layers)
{ 
    this->map_width   = map_width;
    this->map_height  = map_height;
    this->cell_width  = cell_width;
    this->cell_height = cell_height;
    this->num_layers  = num_layers;
    
    for(int i = 0; i < num_layers; i++)
        addLayer();
}

const bool GenericTiledMap::isCellFree(const int cell_x, const int cell_y, const int layer) const
{
    if( layer < 0 || layer >= v_layers.size() )
    {
        return false;
    }

    return (v_layers[layer].isCellFree(cell_x, cell_y));
}

const bool GenericTiledMap::lineIntersection( int start_x,  int start_y,
                                              int end_x,  int end_y, const int layer, 
                                             int *intersection_x, int *intersection_y)
{
    std::vector<GenericTiledMap::Tile> possible_contacts;
    
    /*start_x *= getCellWidth();
    start_y *= getCellHeight();
    end_x   *= getCellWidth();
    end_y   *= getCellHeight();*/
    
    int delta_x = 1;
    int delta_y = 1;
    
    if(start_x > end_x)
        delta_x = -delta_x;
    
    if(start_y > end_y)
        delta_y = -delta_y;
    
    if(end_x - start_x == 0)
    {
        for(int i_y = start_y; i_y != end_y; i_y += delta_y)
        {
            if(!isCellFree(end_x, i_y, layer))
            {
                if(intersection_x != 0)
                    *intersection_x = start_x;
                
                if(intersection_y != 0)
                    *intersection_y = i_y;
                
                return true;
            }
        }
        
        if(intersection_x != 0)
            *intersection_x = -1;
        
        if(intersection_y != 0)
            *intersection_y = -1;
         
        return false;
    }
 
    double m_slope = (double)(end_y*32.0 - start_y*32.0)/(double)(end_x*32.0 - start_x*32.0);

    for(int i_x = start_x*32.0; i_x != end_x*32.0; i_x += delta_x)
    {
        // y - y1 = m(x - x1)
        // y = xm - mx1 + y1
        
        double i_y = (m_slope*i_x) - (m_slope*start_x*32.0) + start_y*32.0; 
    
        if(!isCellFree(i_x/32.0, i_y/32.0, layer))
        {
            if(intersection_x != 0)
                *intersection_x = i_x;
            
            if(intersection_y != 0)
                *intersection_y = i_y;
            
            return true;
        }
    }
    
    if(intersection_x != 0)
        *intersection_x = -1;
    
    if(intersection_y != 0)
        *intersection_y = -1;
    
    return false;
}

void GenericTiledMap::addLayer()
{
    v_layers.push_back( Layer(map_width, map_height) );
}

void GenericTiledMap::setTile(const int cell_x, const int cell_y, const int cell_id, const int layer)
{
    if( layer < 0 || layer >= v_layers.size() )
        return;

    v_layers[layer].setTile(cell_x, cell_y, cell_id);
}

const GenericTiledMap::Tile &GenericTiledMap::getTile(const int cell_x, const int cell_y, const int layer)
{
    if( layer < 0 || layer >= v_layers.size() )
        return Tile(cell_x, cell_y, 0);

    return v_layers[layer].getTile(cell_x, cell_y);
}

GenericTiledMap::Layer &GenericTiledMap::getLayer(const int layer)
{
    return v_layers[layer];
}

const int GenericTiledMap::getLayerIDByName(const std::string name)
{
    for(int i = 0; i < v_layers.size(); i++)
    {
        if(v_layers[i].getName() == name)
            return i;
    }
    
    return -1;
}
