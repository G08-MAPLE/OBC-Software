#ifndef STARTUP_H
#define STARTUP_H

#include <Arduino.h>
#include "main.hpp"
#include "tasks.hpp"
#include "uart.hpp"

static const char *START_TAG = "Start";

void startup(void * param);

#endif