#ifndef ENTITY_H
#define ENTITY_H

/**
* The Entity class is the existential base for any object to be represented
* in the game's logic. */
class Entity
{
private:
    bool should_remove;
    
public:
    
    /**
     * Generic constructor
     * preconditions: none
     * postconditions: none
     * throws: none
     */
    Entity(){ should_remove = false;}
    
    /**
     * Virtual destructor so any inherited
     * classes may achieve true polymorphism
     * preconditions: none
     * postcondiions: none
     * throws: none
     */
    virtual ~Entity(){;}
    
    /**
     * This function must be defined by the child class. 
     * It is invoked on every game cycle.
     * preconditions: none
     * postconditions: the entity in question is updated
     * throws: none
     */
    virtual void onUpdate() { ; }
    
    /**
     * This function must be defined by the child class. 
     * It is invoked on every game's draw cycle.
     * preconditions: none
     * postconditions: the entity in question is drawn
     * throws: none
     */
    virtual void onDraw() { ; }
    
    const bool isRemovable() { return should_remove; }
    
    void removeEntity() { should_remove = true; }
};

#endif