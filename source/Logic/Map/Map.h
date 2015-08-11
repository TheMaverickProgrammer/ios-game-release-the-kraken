#ifndef MAP_H
#define MAP_H

#include <vector>

/**
 * The Map is a representation of an area in the game
 * where entities can and cannot go. 
 *
 * The true representation of a map can be very diverse
 * such as a collection of tiles (grid-based) or collection
 * of polygons (free-roam).
 *
 * This representation can even extend to having a collection of 
 * layers which themselves are simply an ordered collection
 * of the previous objects.
 *
 * The map will later be drawn by the user's specifications.
 */
class Map
{
public:
    
    /**
     * Generic constructor
     * preconditions: None
     * postconditions: None
     * throws:        None
     */
    Map() { ; }
    
    /**
     * Virtual deconstructor
     * preconditions: None
     * postconditions: Memory is de-allocated
     * throws:        None
     */
    virtual ~Map() { ; }
    
    /**
     * Should return true if the point on the map is empty.
     * preconditions: x and y are points located ONLY on the map
     * postconditions: True if the point on the map is empty. Otherwise false.
     * throws:        None
     */
    const virtual bool isCellFree(const int x, const int y, const int layer = 0) const = 0;
};

#endif