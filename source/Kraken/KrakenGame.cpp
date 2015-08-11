#include "KrakenGame.h"

KrakenGame::KrakenGame()
: LogicStateMachine(), EventListener<LogicState>()
{    
    this->pushLogicState(new LoadResourcesGameState());
}

KrakenGame::~KrakenGame() { ; }

void KrakenGame::onEvent(Event<LogicState>* event)
{
    LogicState* next_state = 0;
    
    next_state = event->getData();
    
    this->pushLogicState( next_state );
}