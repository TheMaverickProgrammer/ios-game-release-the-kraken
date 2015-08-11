#ifndef EVENT_PUBLISHER_H
#define EVENT_PUBLISHER_H

#include <vector>
#include "EventListener.h"
#include "NullEventListenerException.h"

#include <iostream>

/**
 * The EventPublisher class is used when an event needs to be invoked
 * regardless if by an object or by another event. By storing EventListeners
 * and then notifying to all (like a boradcast) this can be achieved */
template<typename EVENT_DATA>
class EventPublisher
{
private:
    /** The collection of all listeners to the publisher*/
    std::vector< EventListener<EVENT_DATA>* > v_event_listeners; 

public:
    /**
     * Generic constructor for EventPublisher
     * preconditions: None
     * postconditions: None
     * throws:        None
     */
    EventPublisher<EVENT_DATA>() { ; }
    
    /**
     * Virtual deconstructor. Allows child classes to use true polymorphism.
     * preconditions: None
     * postconditions: The memory for EventPublisher is de-allocated
     * throws:        None
     */
    virtual ~EventPublisher() { ; }
    
    /**
     * This method adds an event listener to it's collection to be notified.
     * (sometimes referred to as "views")
     * preconditions: event_listener is not null
     * postconditions: Adds a listener.
     * throws:         Throws NullEventListenerException if event_listener is null.
     */
    void attach(EventListener<EVENT_DATA> *event_listener) //throw NullEventListenerException
    {
        if(event_listener)
            v_event_listeners.push_back(event_listener);
        else
        {
            throw ( NullEventListenerException() );
        }
    }
    
    /**
     * Any listeners are notified of an event.
     * preconditions: Event is not null.
     * postconditions: If Event is null, nothing. Otherwise, all listeners are notified.
     * throws:        None
     */
    void notify(Event<EVENT_DATA>* event)
    {
        if(event == 0)
            return;
        
        for(int i = 0; i < v_event_listeners.size(); i++)
        {
            v_event_listeners[i]->onEvent(event);
        }
    }
};

#endif