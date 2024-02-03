#ifndef STARTUP_H
#define STARTUP_H

#include <Arduino.h>
#include "main.hpp"
#include "tasks.hpp"
#include "spiffs.hpp"
#include "uart.hpp"

static const char *START_TAG = "Start";

extern UARTController xBeeRadio;            //Make xBeeRadio object visible to other threads
// extern Filesys dartFs;                      //Make dartFs object visible to other threads
extern char data;

void startup(void * param);

#endif