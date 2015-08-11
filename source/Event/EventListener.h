#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "Event.h"

#include <iostream>

/** Forward declaring EventPublisher class*/
// class EventPublisher;

/**
 * A EventListener follows the Observer design pattern such that
 * a publisher will notify any registered listeners of arbitray events. 
 * As a base class, it needs to be derived so it can listen for
 * specific events and act upon them appropriately. */
template<typename EVENT_DATA>
class EventListener
{
private:
    /** The publisher that the listener waits for*/
    // EventPublisher *event_publisher;
    
public:
    /**
     * Generic constructor. Assigns the publisher.
     * precondsitions: None
     * postconditions: None
     * throws:         None
     */
     // EventListener(EventPublisher *publisher){ event_publisher = publisher; }
    EventListener() { ; }
    
    /**
     * Virtual deconstructor. Allows child classes to use true polymorphism.
     * preconditions: None
     * postconditions: The memory for EventListener is de-allocated
     * throws:        None
     */
    virtual ~EventListener(){;}
    
    /**
     * Whenever an event is published, the listener is notified with the event itself.
     * This functions needs to be re-implemented.
     * preconditions: None
     * postconditions: The EventListener will respond to events
     * throws:        None
     */

    virtual void onEvent(Event<EVENT_DATA>* event) = 0;
};

#endif