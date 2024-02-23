#ifndef STARTUP_H
#define STARTUP_H

#include <Arduino.h>
#include "main.hpp"
#include "tasks.hpp"
#include "sd_log.hpp"
#include "uart.hpp"
#include "XBEE.hpp"

static const char *START_TAG = "Start";

// extern Filesys dartFs;                      //Make dartFs object visible to other threads
// extern char* data;

void startup(void * param);

#endif