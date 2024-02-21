#ifndef MAIN_H
#define MAIN_H

#include "tasks.hpp"
#include <Arduino.h>

// Maybe create a namespace to contextualize variables Global?
enum class State {
        BOOT,
        CONFIGURED,
        ONLINE,
        ARMED, 
        LIVE,
        COMPLETE,
        SLEEP,
        };

// Declare global variables as extern so they can be accessed by all files
// For access in other files must include "main.hpp" in that file's header file
extern State state;
extern SemaphoreHandle_t stateMutex;         //State Mutex
extern SemaphoreHandle_t flashMemMutex;      //Memory Mutex
extern SemaphoreHandle_t memBufferMutex;     //Memory Buffer Mutex

#endif