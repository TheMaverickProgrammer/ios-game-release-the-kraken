#ifndef TILEDQT_TILED_MAP_H
#define TILEDQT_TILED_MAP_H

#include "../GenericTiledMap.h"
#include "TiledQT_TMXFileParser.h"

class TiledQT_TiledMap : public GenericTiledMap
{
public:
    
    TiledQT_TiledMap();
    
    void readFromParser(TiledQT_TMXFileParser &parser);
    
    void makeNewMap(const int map_width, const int map_height, 
                    const int cell_width, const int cell_height, const int num_layers = 1);
}; 

#endif