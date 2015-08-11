#ifndef LOGIC_STATE_H
#define LOGIC_STATE_H

class LogicStateMachine;

/** 
 * LogicState is the current state of the app logic itself.
*/

class LogicState
{
public:
    LogicState(){;}
    virtual ~LogicState(){;}
    
    virtual void onUpdate(LogicStateMachine *logic) = 0;
    
    virtual void onDraw() = 0;
};

#endif