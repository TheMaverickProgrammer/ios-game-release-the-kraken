#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <map>

#include <iostream>

class TiledQT_TMXFileParser
{
public:
    
    struct QuickTilesetData
    {
        int cell_width;
        int cell_height;
        int cell_offset_x;
        int cell_offset_y;
        std::string tileset_file_location;
    };
    
    struct QuickTileData
    {
        int cell_x;
        int cell_y;
        int cell_id;
        
        QuickTileData(int x, int y, int id);
        
        // !!!!! -> multi-property array...
    };
    
    typedef std::vector< std::vector<QuickTileData> > QuickTileCollection;
    
    struct QuickLayerData
    {
        bool visible;
        double opacity;
        std::string name;
        
        QuickTileCollection v_tiles;
        
        QuickLayerData();
        
        void resetDimensions(const int map_width, const int map_height);
        
        void setTile(const int cell_x, const int cell_y, int cell_id);
        
        QuickTileData getTile(const int cell_x, const int cell_y);
        
        // !!!!!! -> multi-property array...
    };
    
    typedef std::vector< QuickLayerData > QuickLayerCollection;
    
    struct QuickObjectGroup
    {
        struct QuickObjectData
        {
            int cell_x;
            int cell_y;
            int width;
            int height;
            std::string object_name;
            
            std::map<std::string, std::string> properties;
        };
        
        typedef std::vector<QuickObjectData> QuickObjectCollection;
        
        QuickObjectCollection v_objects;
        std::string      group_name;
        bool             visible;
    };
    
    typedef std::vector<QuickObjectGroup> QuickObjectGroupCollection;
    
    struct QuickMapData
    {
        int map_width;
        int map_height;
        int cell_width;
        int cell_height;
        
        QuickObjectGroupCollection objects;
    };
    
private:
    QuickMapData                     map_data;
    QuickTilesetData                 tileset_data;
    QuickObjectGroupCollection       objectgroups_data;
    QuickLayerCollection layers_data;
    
    int first_GID;
    
public:
    
    const int getFirstGID();
    const int getMapWidth();
    const int getMapHeight();
    const int getNumLayers();
    const int getCellID(int cell_x, int cell_y, int layers);
    const int getCellWidth();
    const int getCellHeight();
    QuickLayerData &getLayer(int layer);
    const std::string &getTilesetFileLocation() ;
    const QuickObjectGroupCollection &getObjectGroups();
    const bool parseTMXFile(const std::string &filename);
};