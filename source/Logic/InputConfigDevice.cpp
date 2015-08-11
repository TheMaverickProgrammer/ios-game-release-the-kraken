/*
 *  InputConfigDevice.h
 *  Jiwon
 *
 *  Created by Brian Peppers on 6/13/11.
 *  Copyright 2011 Brian Peppers. All rights reserved.
 *
 */

#include "InputConfigDevice.h"

namespace Jiwon
{
    // ----------GosuInputRerouteSingleton BEGIN-----------
    
    // Wow... this looks like a train-wreck...
    
    GosuInputRerouteSingleton* GosuInputRerouteSingleton::girs = new GosuInputRerouteSingleton();
        
    void GosuInputRerouteSingleton::setInput(Gosu::Input* inputDevice)
    {
        input = inputDevice;
    }
    
    void GosuInputRerouteSingleton::buttonDown(Gosu::Button button)
    {
        vButtonPresses.push_back(button);
    }
    
    const std::vector<Gosu::Button> *GosuInputRerouteSingleton::buttonPresses() const
    { return &vButtonPresses; }
    
    void GosuInputRerouteSingleton::clear()
    {
        vButtonPresses.clear();
    }
        
    GosuInputRerouteSingleton::GosuInputRerouteSingleton()
    {
        input = 0;
    }
    
    GosuInputRerouteSingleton::~GosuInputRerouteSingleton()
    {
        delete girs;
        girs = 0;
    }
    
    // ----------GosuInputRerouteSingleton END-----------
    
    // ----------InputConfigDevice BEGIN-----------
    
    InputConfigDevice::InputConfigDevice(bool virtualDeviceEnabled) 
    {
        input = 0;
        virtualDevice = virtualDeviceEnabled;
        virtualClean = 0;
        lockDevice = false;
        this->girs = GosuInputRerouteSingleton::girs;
    }

    InputConfigDevice::~InputConfigDevice() 
    {
        ; 
    }

    void InputConfigDevice::enableVirtualDevice(bool virtualDeviceEnabled)
    {
        virtualDevice = virtualDeviceEnabled;
    }
    
    void InputConfigDevice::setInput(Gosu::Input* inputDevice)
    {
        input = inputDevice;
    }

    void InputConfigDevice::bindKey(Gosu::Button button, std::string event)
    {        
        vKeyEvents.insert(std::make_pair(event, button));
    }
    
    void InputConfigDevice::virtualButtonTouch(Gosu::Button button)
    {
        // Iterate for virtual button presses from input
        for(std::map<std::string, Gosu::Button>::iterator i = vKeyEvents.begin();
            i != vKeyEvents.end(); i++)
        {
            if(i->second == button)
                vVirtualBuffer.push_back(i->first);
        }
    }

    void InputConfigDevice::virtualEventTrigger(std::string event)
    {
        vVirtualBuffer.push_back(event);
    }
    
    const std::vector<std::string> &InputConfigDevice::triggeredEvents()
    {
        return vTriggeredEvents;
    }
    
    const bool InputConfigDevice::findEvent ( const std::string item)
    {
        std::vector<std::string>::iterator iter = vTriggeredEvents.end();
        iter = std::find(vTriggeredEvents.begin(), vTriggeredEvents.end(), item);
        
        if(iter != vTriggeredEvents.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void InputConfigDevice::lock()
    {
        lockDevice = true;
    }
    
    void InputConfigDevice::unlock()
    {
        lockDevice = false;
    }
    
    void InputConfigDevice::poll()
    {        
        // Cleanup immediately
        if(!vTriggeredEvents.empty())
            vTriggeredEvents.clear();
        
        if(lockDevice)
            return;
        
        if(!virtualDevice)
        {            
            // Iterate for real button presses from input
            for(std::map<std::string, Gosu::Button>::iterator i = vKeyEvents.begin();
                i != vKeyEvents.end(); i++)
            {
               // /// std::cout << i->first << std::endl;
                
                if(input->down(i->second))
                {
                    vTriggeredEvents.push_back(i->first);
                }
            }
        }
        else
        {
            for(std::vector<std::string>::iterator i = vVirtualBuffer.begin(); i != vVirtualBuffer.end(); i++)
            {
                vTriggeredEvents.push_back((*i));
            }
            
            vVirtualBuffer.clear();
        }
    }

    void InputConfigDevice::reset()
    {
        vKeyEvents.clear();
        vTriggeredEvents.clear();
    }
    
    const std::vector<Gosu::Button> *InputConfigDevice::buttonPresses() const
    {
        return this->girs->buttonPresses();
    }
    
    // ----------InputConfigDevice END-----------
}