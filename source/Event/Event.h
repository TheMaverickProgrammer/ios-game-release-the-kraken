#ifndef EVENT_H
#define EVENT_H

/** 
* The Event class is an event that occurs and needs to be spread 
* To any listener waiting for the event so they may respond appropriately.
* As a base class, it can be derived to add more features. 
* However, as a generic, it can also be used effecienty exactly as it is.
*/
template<typename EVENT_DATA>
class Event
{
private:
    /** The data to the Event*/
    EVENT_DATA *data;
    
public:
    /**
     * Generic constructor. Sets the data to the event.
     * preconditions: None
     * postconditions: data is assigned to event_data
     * throws:        None
     */
    Event(EVENT_DATA *event_data) {data = event_data;}
    
    /**
     * Virtual deconstructor.
     * preconditions: None
     * postconditions: The memory for Event is de-allocated.
     * throws:        None
     */
    virtual ~Event() { delete data; data = 0;}
    
    /** 
     * Returns pointer to the event's data.
     * preconditions: None
     * postconditions: pointer of EVENT_DATA is returned
     * throws:        None
     */
    EVENT_DATA *getData() { return data; }
};

#endif