#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "state_machine.hpp"
#include <stdio.h>
#include "esp_log.h"

// State machine based off of Switch case of this article:
//https://silverweed.github.io/Functional_State_Machines_in_C++/

StateMachine::StateMachine() {
    state = State::BOOT;                                    // Set initial state to Boot no peripherals configured
    ESP_LOGI(SM_TAG, "State Machine Object Created");
}

void StateMachine::_poweredState() {
    //If heartbeat thread exists kill it
    if (state == State::BOOT) {
        //TODO (Low priority) check if UART already exists, then can skip UART init() in system check
    }

    int systemStatus;
    //TODO: Check if all systems online
    //Basic systems:
    //  UART            reutrn 0 if good, -1 if bad
    //  Accelerometer   return 0 if good, -1 if bad
    //  Save to SD      return 0 if good, -1 if bad
    systemStatus = 0; //TODO = sum of system checks
    ESP_LOGI(SM_TAG, "Status: POWERED");
    if (systemStatus == 0) {
        ESP_LOGI(SM_TAG, "State: POWERED");
        ESP_LOGI(SM_TAG, "Changing States");
        state = State::ONLINE;
    }
    else {
        ESP_LOGI(SM_TAG, "Changing States");
        state = State::CONFIGURED;
    }
}

void StateMachine::_onlineState() {
    //TODO: Create XBee Rx thread
    //  Check if UART object exists
    //  Should just read Rx buffer and parse data as fast as possible
    //  Create watchdog timer for heatbeat functionality
    //  If timer 1/2 filled and Tx buffer empty send dummy data (heartbeat)
    //  Reset watchdog each time Rx buffer Rx's data (check if empty?)  
    //  Will call more tasks to parse, etc data

    //TODO: Find way of communicating Response from Rx thread (Global var?)
    // if (Response is received from GND control) {
    //      state = State::ARMED;
    //}
    // 
    // else if (Watchdog is triggered) {
    //      state = State::POWERED;
    // }

    // else {
    //     do nothing, will this cause the program to hang?
    // }

    // Following used for testing state machine
    ESP_LOGI(SM_TAG, "State: ONLINE");
    ESP_LOGI(SM_TAG, "Changing States");
    state = State::ARMED;
}

void StateMachine::_armedState() {
    //TODO: Find way of communicating Response from Rx thread (Global var?)
    // if (Response is received from GND control) {
    //      state = State::LIVE;
    //}
    // 
    // else if (Watchdog is triggered) {
    //      state = State::POWERED;
    // }

    // else {
    //     do nothing, will this cause the program to hang?
    // }
    ESP_LOGI(SM_TAG, "State: ARMED");
    ESP_LOGI(SM_TAG, "Changing States");
    state = State::LIVE;
}

void StateMachine::_liveState() {
    // Once live state has been acheived system will no longer regress to powered state on
    // watchdog timeout. Once the burn wire has been triggered must record to the best of its 
    // abilities. If system fails or is destroyed during crash, system will stay in Live state

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
    ESP_LOGI(SM_TAG, "State: LIVE");
    ESP_LOGI(SM_TAG, "Changing States");
    state = State::COMPLETE;
}

void StateMachine::_testCompleteState() {

    //Time delay make sure all relevant data is recorded (include in impact sensing logic for clarity?)

    //kill all sensor threads
    //log time of test completion
    //Tx test completion msg to GND station (also ensures all data has been cleared from Tx buffer)
    
    //On Rx'ing confirmation from GND
    ESP_LOGI(SM_TAG, "State: TEST COMPLETE");
    ESP_LOGI(SM_TAG, "Changing States");
    state = State::SLEEP;
}

void StateMachine::_sleepState() {
    //Set ucontroller kill pin high/low to disconnect batteries
    ESP_LOGI(SM_TAG, "State: SLEEP");
    //end of program execution
}

void StateMachine::update() {
    //Switch structure to check state this will be run by the RTOS task
    switch (state) {
        case (State::BOOT):         //Short hand notation, call same function as Powered case
        case State::CONFIGURED:
            _poweredState();
            break;
        case State::ONLINE:
            _onlineState();
            break;
        case State::ARMED:
            _armedState();
            break;
        case State::LIVE:
            _liveState();
            break;
        case State::COMPLETE:
            _testCompleteState();
            break;
        case State::SLEEP:
            _sleepState();
            break;
    }
}
