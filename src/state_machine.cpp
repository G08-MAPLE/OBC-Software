#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "state_machine.hpp"
#include <stdio.h>
#include "esp_log.h"
         
StateMachine::StateMachine() {
    currentState = State::Boot;
    ESP_LOGI(SM_TAG, "State Machine Object Created");
    //https://stackoverflow.com/questions/30228452/function-pointer-within-class
}

void StateMachine::_poweredState() {
    //If heartbeat thread exists kill it
    if (currentState == State::Boot) {
        //TODO (Low priority) check if UART already exists, then can skip UART init() in system check
    }
    

    //Add threadsafe checking procedure
    //Define next/fail states for clarity
    nextState = State::SystemOnline;
    failState = State::Powered;
    //--------------

    int systemStatus;
    //TODO: Check if all systems online
    //Basic systems:
    //  UART            reutrn 0 if good, -1 if bad
    //  Accelerometer   return 0 if good, -1 if bad
    //  Save to SD      return 0 if good, -1 if bad
    systemStatus = 0; //TODO = sum of system checks
    ESP_LOGI(SM_TAG, "Status: POWERED");
    if (systemStatus == 0) {
        //Add threadsafe checking procedure
        // currentState = nextState;
        ESP_LOGI(SM_TAG, "State: POWERED");
        ESP_LOGI(SM_TAG, "Changing States");
        currentState = nextState;
    }
    else {
        //Add threadsafe checking procedure
        // currentState = failState;
        ESP_LOGI(SM_TAG, "Changing States");
        currentState = failState;
    }
}

void StateMachine::_systemOnlineState() {
    //Add threadsafe checking procedure
    //Define next/fail states for clarity
    nextState = State::Armed;
    failState = State::Powered;
    //--------------

    //TODO: Create XBee Rx thread
    //  Check if UART object exists
    //  Should just read Rx buffer and parse data as fast as possible
    //  Create watchdog timer for heatbeat functionality
    //  If timer 1/2 filled and Tx buffer empty send dummy data (heartbeat)
    //  Reset watchdog each time Rx buffer Rx's data (check if empty?)  
    //  Will call more tasks to parse, etc data

    //TODO: Find way of communicating Response from Rx thread (Global var?)
    // if (Response is received from GND control) {
    //     currentState = nextState;
    //      state = StateMachine::armedState;
    //}
    // 
    // else if (Watchdog is triggered) {
    //     currentState = failState;
    //      state = StateMachine::poweredState;
    // }

    // else {
    //     do nothing, will this cause the program to hang?
    // }
    ESP_LOGI(SM_TAG, "State: ONLINE");
    ESP_LOGI(SM_TAG, "Changing States");
    currentState = nextState;
}

void StateMachine::_armedState() {
    //Add threadsafe checking procedure
    //Define next/fail states for clarity
    nextState = State::Live;
    failState = State::Powered;
    //--------------

    //TODO: Find way of communicating Response from Rx thread (Global var?)
    // if (Response is received from GND control) {
    //     currentState = nextState;
    //      state = StateMachine::liveState;
    //}
    // 
    // else if (Watchdog is triggered) {
    //     currentState = failState;
    //      state = StateMachine::poweredState;
    // }

    // else {
    //     do nothing, will this cause the program to hang?
    // }
    ESP_LOGI(SM_TAG, "State: ARMED");
    ESP_LOGI(SM_TAG, "Changing States");
    currentState = nextState;
}

void StateMachine::_liveState() {
    //Add threadsafe checking procedure
    //Define next/fail states for clarity
    // nextState = State::TestComplete;
    // failState = State::Live;
    // Once live state has been acheived system will no longer regress to powered state
    // Once the burn wire has been triggered must record to the best of its abilities
    // If system fails or is destroyed during crash, system will stay in Live state
    //--------------

    // start measurement threads
    //     Accelerometer
    //     Magnetometer
    //     IMU
    //     Temperature
    //     Camera
    //     Log to mem./SD card
    //     Burn wire
    // Each thread will write bytes to the UART XBee this will require thread synchronization
    // so that data is not corrupted by the multiple threads mutex/semaphore/etc.
    // each thread will also write to the SD card so that memory is logged internally

    // Sense impact (Logic to be determined)
    // After impact move to next state
    // currentState = nextState;
    ESP_LOGI(SM_TAG, "State: LIVE");
    ESP_LOGI(SM_TAG, "Changing States");
    currentState = State::TestComplete;
}

void StateMachine::_testCompleteState() {
    //Add threadsafe checking procedure
    //Define next/fail states for clarity
    // nextState = State::Sleep;
    //--------------

    //Time delay make sure all relevant data is recorded

    //kill all sensor threads
    //log time of test completion
    //Tx test completion msg to GND station (also ensures all data has been cleared from Tx buffer)
    
    //On Rx'ing confirmation from GND
    // currentState = nextState;
    ESP_LOGI(SM_TAG, "State: TEST COMPLETE");
    ESP_LOGI(SM_TAG, "Changing States");
    currentState = State::Sleep;
}

void StateMachine::_sleepState() {
    //Set ucontroller kill pin high/low to disconnect batteries
    ESP_LOGI(SM_TAG, "State: SLEEP");
    //end of program execution
}

void StateMachine::update() {
    //Switch structure to check state this will be run by the RTOS task
    switch (currentState) {
        case (State::Boot):         //Short hand notation will continue to Powered case since they are the same.
        case State::Powered:
            _poweredState();
            break;
        case State::SystemOnline:
            _systemOnlineState();
            break;
        case State::Armed:
            _armedState();
            break;
        case State::Live:
            _liveState();
            break;
        case State::TestComplete:
            _testCompleteState();
            break;
        case State::Sleep:
            _sleepState();
            break;

    }
}
