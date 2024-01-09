#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// inline State currentState;
// inline State nextState;
// inline State failState;
static const char *SM_TAG = "State_Machine";

class StateMachine
{
//private
    enum class State {
        Boot,
        Powered, 
        SystemOnline, 
        Armed, 
        Live, 
        TestComplete, 
        Sleep,
        };

public:
    //void (*state)();
    State currentState;
    State nextState;
    State failState;
    StateMachine();
    void _poweredState();
    void _systemOnlineState();
    void _armedState();
    void _liveState();
    void _testCompleteState();
    void _sleepState();
    void update();
};
#endif