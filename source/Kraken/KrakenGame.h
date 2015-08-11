#ifndef Kraken_ADVENTURE_GAME_H
#define Kraken_ADVENTURE_GAME_H

#include "Dependencies.h"
#include "LogicStateMachine.h"
#include "EventListener.h"
#include "GameStates.h"

class KrakenGame : public LogicStateMachine, public EventListener<LogicState>
{
private:
    
public:
    KrakenGame();
    ~KrakenGame();
    
    void onEvent(Event<LogicState>* event);
};

#endif