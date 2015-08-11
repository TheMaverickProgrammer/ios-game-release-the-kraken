#include "TiledQT_TiledMap.h"
    
TiledQT_TiledMap::TiledQT_TiledMap() { ; }

void TiledQT_TiledMap::readFromParser(TiledQT_TMXFileParser &parser)
{ 
    makeNewMap( parser.getMapWidth(), parser.getMapHeight(),
               parser.getCellWidth(), parser.getCellHeight(),
               parser.getNumLayers() );
    
    for(int layer_i = 0; layer_i < parser.getNumLayers(); layer_i++)
    {            
        for(int cell_y = 0; cell_y < parser.getMapHeight(); cell_y++)
        {                
            for(int cell_x = 0; cell_x < parser.getMapWidth(); cell_x++)
            {
                setTile(cell_x, cell_y, parser.getCellID(cell_x, cell_y, layer_i), layer_i );
            }
        }
    }
}

void TiledQT_TiledMap::makeNewMap(const int map_width, const int map_height, 
                const int cell_width, const int cell_height, const int num_layers)
{
    setAttributes(map_width, map_height, cell_width, cell_height, num_layers);
}
