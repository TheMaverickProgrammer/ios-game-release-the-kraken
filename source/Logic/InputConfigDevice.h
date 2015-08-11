/*
 *  InputConfigDevice.h
 *  Jiwon
 *
 *  Created by Brian Peppers on 6/13/11.
 *  Copyright 2011 Brian Peppers. All rights reserved.
 *
 */

#ifndef INPUT_CONFIG_DEVICE_H
#define INPUT_CONFIG_DEVICE_H

#include "Requirements.h"

namespace Jiwon
{	
    /*
     Below is a singleton for the InputConfigDevice to route the
     Input::buttonDown pointer (typeid boost::function<>) to
     a function that all devices can use. The idea behind this
     is to allow the devices to return current button press IDs.
     
     i.e. Joystick configuration screen:
     {
         <animation playing> JUMP BUTTON | N/A
     
         !-- then highlight and press a button on the keyboard/gamepad
     
         <animation playing> JUMP BUTTON | GP BUTTON 1
     }
     
     This simple idea, however became complex because of Gosu's design
     of routing the buttonDown function that allows the user to know
     what button is being held down.
     */
    
    class GosuInputRerouteSingleton
    {
    private:
        Gosu::Input *input;
        std::vector<Gosu::Button> vButtonPresses;
    
    public:
        static GosuInputRerouteSingleton *girs;
        
        const std::vector<Gosu::Button> *buttonPresses() const;
        
        void setInput(Gosu::Input*);
        void buttonDown(Gosu::Button);
        void clear();
        
        GosuInputRerouteSingleton();
        ~GosuInputRerouteSingleton();
    };
    
	/*!
	 The InputConfigDevice allows keypresses to flag "events"
     so that any form of input can be configured to the user
     (where it is the programmer or player) and his/her liking.
     
     Also adds for AI controller setup. A virtual key-stroke can 
     flag these events and controll objects and characters easily.
     */
    
	class InputConfigDevice
    {
        friend class GosuInputRerouteSingleton;
        
    protected:
        std::multimap<std::string, Gosu::Button> vKeyEvents;
        std::vector<std::string> vTriggeredEvents;
       
        Gosu::Input *input;
       
        std::vector<std::string> vVirtualBuffer;
        bool virtualDevice;
        bool lockDevice; // If locked, nothing is triggered.
        int virtualClean;
        
        GosuInputRerouteSingleton* girs;

    public:
        InputConfigDevice(bool virtualDeviceEnabled = false);
        ~InputConfigDevice();
        
        /*
        // List all events {DEBUG}
        void debugPrint(){
           for(std::vector<std::string>::iterator i = vTriggeredEvents.begin(); i != vTriggeredEvents.end(); i++)
           {
               /// std::cout << (*i) << std::endl;
           }
        }*/
        
        void setInput(Gosu::Input*);
        void bindKey(Gosu::Button, std::string);
        const std::vector<std::string> &triggeredEvents();
        void virtualButtonTouch(Gosu::Button);
        void virtualEventTrigger(std::string);
        const bool findEvent (const std::string);
        void enableVirtualDevice(bool virtualDeviceEnabled = true);
        const std::vector<Gosu::Button> *buttonPresses() const;
        void lock();
        void unlock();
        
        const bool isLocked() const { return lockDevice; }
        
        void poll();
        void reset();
    };
}

#endif
