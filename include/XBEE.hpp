#ifndef XBEE_H
#define XBEE_H

#include "main.hpp"
#include "spiffs.hpp"
#include "startup.hpp"

void XBEE_rx(void * param);
UARTController* getXBeeRadio();

#endif