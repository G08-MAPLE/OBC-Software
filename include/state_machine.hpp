#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


static const char *SM_TAG = "State_Machine";

class StateMachine
{
    enum class State {
        BOOT,
        POWERED, 
        ONLINE, 
        ARMED, 
        LIVE, 
        TESTCOMPLETE, 
        SLEEP,
        DEAD,        //Testing state so while loop doesn't print infinately (should be replaced with vTaskDelete())
        };
private:
    void _poweredState();
    void _onlineState();
    void _armedState();
    void _liveState();
    void _testCompleteState();
    void _sleepState();
public:
    State state;     //State may need to become a global variable is "worker threads" require state
    StateMachine();
    void update();  //Function that will run in loop to update state machine
};
#endif