/*
 *  Sequence.cpp
 *  Jiwon
 *
 *  Created by Brian M. Peppers on 7/10/11.
 *  Copyright 2011 Brian M. Peppers. All rights reserved.
 *
 */

#include "Sequence.h"

namespace Jiwon
{
    Sequence::Sequence() { ; }
    
    Sequence &Sequence::operator=(const Sequence &rhs)
    {
        this->eventFunctions = rhs.eventFunctions;
        
        /*
         REDUNDANT CODE:
         
         this->eventFunctions.clear();
        
        foreach(boost::function<bool ()> e, rhs.eventFunctions)
            this->eventFunctions.push_back(e);*/
    }
    
    Sequence::~Sequence() 
    {
        /*
         I am told this code is redundant since ~deque pops elements
         
         CODE:
            foreach(boost::function<bool ()> e, eventFunctions)
                eventFunctions.pop_front();
         */
    }
 
    void Sequence::addEventFunction(boost::function<bool ()> e)
    {
        // Push the event at the end of the double-ended queue
        eventFunctions.push_back(e);
    }
 
    void Sequence::update()
    {
 
        /*If the event function returns true, 
        the event has completed succesfully.*/
 
        if(eventFunctions.front()())
        {
             // Push the event to the end (to repeat the sequence)
             eventFunctions.push_back(eventFunctions.front());
 
             // Remove this event from the front
             eventFunctions.pop_front();
        }
    }
};