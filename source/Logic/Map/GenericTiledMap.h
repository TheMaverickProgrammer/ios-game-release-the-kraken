#ifndef GENERIC_TILED_MAP_H
#define GENERIC_TILED_MAP_H

#include "Map.h"

#include <iostream>

class GenericTiledMap : public Map
{
public:
    /** 
     * Struct of a Tile which has a location
     * (x and y) as well as an ID (for the tileset)
     */
    class Tile
    {
    private:
        int cell_x;
        int cell_y;
        int cell_id;
        
    public:
        /** Just set everything to falty values. **/
        Tile();
        
        /** Set specific values for the tile via constructor. **/
        Tile(const int cell_x, const int cell_y, const int cell_id);
        
        Tile(const Tile &other);
        
        void setCellID(int cell_id);
        
        const int getCellID() const;
        
        const int getCellX();
        const int getCellY();
        
        virtual ~Tile();
    };
    
    typedef std::vector< std::vector<GenericTiledMap::Tile> > TileMutliArray;
    
    class Layer
    {
    private:
        /** vector of vector of Tiles. i.e. multi-array. **/
        TileMutliArray v_tiles;
        
    public:
        
        float opacity;
        bool visible;
        std::string name;
        
        Layer(const int map_width, const int map_height);
        
        virtual ~Layer();
        
        void resetDimensions(const int map_width, const int map_height);
        
        /**
         * Return true if the point on the map is empty. (tiles[y][x] <= 0)
         * preconditions: x and y are CELL points located ONLY on the map
         * postconditions: True if the point on the map is empty. Otherwise false.
         * throws:        None
         */
        const bool isCellFree(const int cell_x, const int cell_y) const;
        
        /**
         * Replaces an existing tile ID to a new ID
         * preconditions: x and y are CELL points located ONLY on the map
         * postconditions: Tile at x and y has the new ID of cell_id
         * throws:        None
         */
        void setTile(const int cell_x, const int cell_y, int cell_id);
        
        const Tile &getTile(const int cell_x, const int cell_y);
        
        void setName(std::string name);
        
        const std::string &getName();
    };
    
    typedef std::vector<Layer> LayerCollection;
    
private:
    int map_width, map_height, cell_width, cell_height, num_layers;
    
    LayerCollection v_layers;
    
public:
    /**
     * Generic constructor that sets map values to
     * the following defaults:
     * Map width   => 40 cells (640px)
     * Map height  => 30 cells (480px)
     * Cell width  => 16px
     * Cell height => 16px
     * # of Layers => 1
     */
    GenericTiledMap();
    
    /**
     * Constructor which takes in the size of the map
     * in cells as well as the cell's dimensions.
     * preconditions: all values are real numbers
     * postconditions: vector of tiles is allocated
                       to reflect size of the map
     * throws:        None
     */
    GenericTiledMap(const int map_width, const int map_height, 
                    const int cell_width, const int cell_height, const int num_layers = 1);
    
    void setAttributes(const int map_width, const int map_height, 
                       const int cell_width, const int cell_height, const int num_layers = 1);

    
    /**
     * Virtual deconstructor
     * preconditions: None
     * postconditions: Memory is de-allocated
     * throws:        None
     */
    virtual ~GenericTiledMap();
    
    const int getMapWidth();
    const int getMapHeight();
    const int getNumLayers();
    const int getCellWidth();
    const int getCellHeight();
    
    /**
     * Return true if the point on the map is empty. (tiles[y][x] <= 0)
     * preconditions: x and y are CELL points located ONLY on the map. Also, layer # exist.
     * postconditions: True if the point on the map is empty. Otherwise false.
     * throws:        None
     */
    const bool isCellFree(const int cell_x, const int cell_y, const int layer) const;
    
    const bool lineIntersection( int start_x,  int start_y,
                                 int end_x,  int end_y, const int layer, 
                                int *intersection_x, int *intersection_y);
    
    void addLayer();
    
    /**
     * Replaces an existing tile ID to a new ID
     * preconditions: x and y are CELL points located ONLY on the map. Also, layer # exist.
     * postconditions: Tile at x and y has the new ID of cell_id
     * throws:        None
     */
    void setTile(const int cell_x, const int cell_y, const int cell_id, const int layer);
    
    /**
     * Returns existing tile data.
     * preconditions: x and y are CELL points located ONLY on the map. Also, layer # exists.
     * postconditions: Tile at x and y is returned. Otherwise bad data is returned.
     * throws:        None
     */
    const Tile &getTile(const int cell_x, const int cell_y, const int layer);
    
    Layer &getLayer(const int layer);
    
    const int getLayerIDByName(const std::string name);
};

#endif