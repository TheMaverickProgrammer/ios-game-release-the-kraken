#include "TiledQT_TMXFileParser.h"

// TILE DATA FUNCTIONS
TiledQT_TMXFileParser::QuickTileData::QuickTileData(int x, int y, int id)
{
    cell_x = x;
    cell_y = y;
    cell_id = id;
}

// LAYER DATA FUNCTIONS
TiledQT_TMXFileParser::QuickLayerData::QuickLayerData()
{
    visible = true;
    opacity = 1.0;
    name = "layer";
}

void TiledQT_TMXFileParser::QuickLayerData::resetDimensions(const int map_width, const int map_height)
{
    for(int i = 0; i < map_height; i++)
    {
        v_tiles.push_back( std::vector<QuickTileData>() );
        
        for(int j = 0; j < map_width; j++)
        {
            v_tiles[i].push_back( QuickTileData(j, i, 0) );
        }
    }
}

void TiledQT_TMXFileParser::QuickLayerData::setTile(const int cell_x, const int cell_y, int cell_id)
{
    if(v_tiles.empty() )
        return;
    
    if(cell_y < 0 || cell_y >= v_tiles.size())
        return;
    
    if(cell_x < 0 || cell_x >= v_tiles[0].size())
        return;
    
    v_tiles[cell_y][cell_x].cell_id = cell_id;
}

TiledQT_TMXFileParser::QuickTileData TiledQT_TMXFileParser::QuickLayerData::getTile(const int cell_x, const int cell_y)
{
    if(cell_y < 0 || cell_y >= v_tiles.size())
        return TiledQT_TMXFileParser::QuickTileData(cell_x, cell_y, 0);
    
    if(cell_x < 0 || cell_x >= v_tiles[0].size())
        return TiledQT_TMXFileParser::QuickTileData(cell_x, cell_y, 0);
    
    return (v_tiles[cell_y][cell_x]);
}

// FILE PARSER FUNCTIONS
const int TiledQT_TMXFileParser::getFirstGID()
{
return first_GID;
}

const int TiledQT_TMXFileParser::getMapWidth()
{
return map_data.map_width;
}

const int TiledQT_TMXFileParser::getMapHeight()
{
return map_data.map_height;
}

const int TiledQT_TMXFileParser::getNumLayers()
{
return (int)layers_data.size();
}

const int TiledQT_TMXFileParser::getCellID(int cell_x, int cell_y, int layers)
{
return (layers_data[layers].getTile(cell_x, cell_y).cell_id);
}

const int TiledQT_TMXFileParser::getCellWidth()
{
return tileset_data.cell_width;
}

const int TiledQT_TMXFileParser::getCellHeight()
{
return tileset_data.cell_height;
}

TiledQT_TMXFileParser::QuickLayerData &TiledQT_TMXFileParser::getLayer(int layer)
{
return layers_data[layer];
}

const std::string &TiledQT_TMXFileParser::getTilesetFileLocation() 
{
return tileset_data.tileset_file_location;
}

const TiledQT_TMXFileParser::QuickObjectGroupCollection &TiledQT_TMXFileParser::getObjectGroups() 
{
return objectgroups_data;
}

const bool TiledQT_TMXFileParser::parseTMXFile(const std::string &filename)
{            
boost::property_tree::ptree pt;
boost::property_tree::xml_parser::read_xml(filename, pt);

map_data.map_width   = pt.get<int>( "map.<xmlattr>.width", 0 );
map_data.map_height  = pt.get<int>( "map.<xmlattr>.height", 0 );
map_data.cell_width  = pt.get<int>( "map.<xmlattr>.tilewidth", 0 );
map_data.cell_height = pt.get<int>( "map.<xmlattr>.tileheight", 0 );

BOOST_FOREACH( boost::property_tree::ptree::value_type &v, pt.get_child("map") ) 
{
    if(v.first == "tileset") 
    {
        first_GID                = v.second.get<int>("<xmlattr>.firstgid", 0);
        
        tileset_data.cell_width  = v.second.get<int>("<xmlattr>.tilewidth", 0);
        tileset_data.cell_height = v.second.get<int>("<xmlattr>.tileheight", 0);
        tileset_data.tileset_file_location  = v.second.get<std::string>("image.<xmlattr>.source");
        
        // !!!!!! -> multiple tilesets?
    }
}

BOOST_FOREACH( boost::property_tree::ptree::value_type &v, pt.get_child("map") ) 
{
    if(v.first == "layer")
    {                
        QuickLayerData layer;
        layer.resetDimensions(map_data.map_width, map_data.map_height);
        
        layer.name    = v.second.get<std::string>("<xmlattr>.name");
        
        layer.opacity = v.second.get<double> ("<xmlattr>.opacity", 1);
        layer.visible = v.second.get<bool>  ("<xmlattr>.visible", true);
        
        std::string csv = v.second.get<std::string>("data");
        
        typedef boost::char_separator<char> sep;
        typedef boost::tokenizer< sep > tk;
        
        tk tokens(csv, sep(",\n\r"));
        int index = 0;
        
        int x = 0;
        int y = 0;
        
        for (tk::iterator i(tokens.begin()); i!=tokens.end(); ++i) 
        {
            layer.setTile( x, y, boost::lexical_cast<int>( *i ) );
            
            if(++x >= map_data.map_width)
            {
                y++;
                x = 0;
            }
            
            index++;
        }
        
        layers_data.push_back( layer );
    }
    
}    

BOOST_FOREACH( boost::property_tree::ptree::value_type &v, pt.get_child("map") )
{
    if(v.first == "objectgroup") 
    {               
        QuickObjectGroup group;
        
        group.group_name  = v.second.get<std::string>("<xmlattr>.name");
        
        BOOST_FOREACH( boost::property_tree::ptree::value_type &o, v.second ) 
        {
            if(o.first == "object") 
            {
                QuickObjectGroup::QuickObjectData object;
                
                object.object_name = o.second.get<std::string>( "<xmlattr>.name", "Object" );
                
                object.cell_x = o.second.get<int>( "<xmlattr>.x" );
                
                object.cell_y = o.second.get<int>( "<xmlattr>.y" );
                
                // If no width to be found, it is a tiled object and therefore is the size if a tile
                object.width  = o.second.get<int>( "<xmlattr>.width", tileset_data.cell_width );
                object.height = o.second.get<int>( "<xmlattr>.height", tileset_data.cell_height );
                
                // LOOK FOR OBJECT PROPERTIES
                BOOST_FOREACH( boost::property_tree::ptree::value_type &pg, o.second ) 
                {
                    if(pg.first == "properties") 
                    {
                        BOOST_FOREACH( boost::property_tree::ptree::value_type &p, pg.second ) 
                        {
                            if(p.first == "property") 
                            {
                                std::string property_name, property_value;
                                
                                property_name = p.second.get<std::string>( "<xmlattr>.name" );
                                property_value = p.second.get<std::string>( "<xmlattr>.value" );
                                
                                object.properties.insert(std::make_pair(property_name, property_value));
                            }
                        }
                    }
                }
                
                group.v_objects.push_back( object );
            }
        }
        
        objectgroups_data.push_back( group );
    }
}

return true; // Parsing succeeded
}