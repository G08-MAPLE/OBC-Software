#ifndef MAIN_H
#define MAIN_H

#include "tasks.hpp"

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

inline State state;

#endif