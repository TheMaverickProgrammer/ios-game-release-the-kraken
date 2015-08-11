#include "EntityGroup.h"

EntityGroup::EntityGroup() { ; }
EntityGroup::~EntityGroup() { ; }

void EntityGroup::update()
{
    for(int i = 0; i < getSize(); i++)
    {
        get(i)->onUpdate();
        
        if(get(i)->isRemovable())
        {
            remove(i);
            continue;
        }
    }
            
}

void EntityGroup::draw()
{
    for(int i = 0; i < getSize(); i++)
        get(i)->onDraw();
}
