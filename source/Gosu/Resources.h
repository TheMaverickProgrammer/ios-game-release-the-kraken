#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <string>

template<typename T>
class Resources
{
private:
    std::map<std::string, T> resources_map;
 
public:
    Resources() { ; }
    ~Resources() { ; }

    void bind(std::string identifier, T resource_t)
    {
       // resources_map.insert(std::pair<std::string, T>(identifier, resource_t));
        resources_map[identifier] = resource_t;
    }

    const bool contains(std::string identifier)
    {
        return ( resources_map.find(identifier) != resources_map.end() );
    }

    void remove(std::string identifier)
    {
        resources_map.erase( resources_map.find(identifier) );
    }
    
    T get(std::string identifier)
    {
        T ptrResource;
        
        if(contains(identifier))
        {
            ptrResource = resources_map[identifier];
        }
        
        return ptrResource;
    }
};

#endif