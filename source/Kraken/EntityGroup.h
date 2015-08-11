#ifndef ENTITY_GROUP_H
#define ENTITY_GROUP_H

#include "Group.h"
#include "../Logic/Entity.h"

class EntityGroup : public Group<Entity*>
{
public:
    
    EntityGroup();
    ~EntityGroup();
    
    void update();
    
    void draw();
};

#endif