#ifndef LOGIC_STATE_MACHINE_H
#define LOGIC_STATE_MACHINE_H

#include "LogicState.h"

#include <list>

/**
 * The LogicStateMachine class is the core of the logic for any app. 
 * The creator of the app can use this as the overall shell
 * and determine themselves how the states will be used*/
class LogicStateMachine
{
private:
    std::list<LogicState*> l_logic_states;
    
public:
    LogicStateMachine(){;}
    virtual ~LogicStateMachine(){;}
    
    void update()
    { if(!l_logic_states.empty()) l_logic_states.front()->onUpdate(this); }
    
    void draw()
    { if(!l_logic_states.empty()) l_logic_states.front()->onDraw(); }
    
    void pushLogicState(LogicState *state)
    { l_logic_states.push_front(state); }
    
    const bool isEmpty() { return (l_logic_states.size() == 0); }
    
    LogicState *popLogicState()
    { 
        LogicState* back_value = l_logic_states.front(); 
        l_logic_states.pop_front();
        
        return back_value;
    }
};

#endif