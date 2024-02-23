#ifndef XBEE_Rx_H
#define XBEE_Rx_H

#include "main.hpp"
#include "spiffs.hpp"
#include "startup.hpp"
#include "uart.hpp"

void XBEE_rx(void * param);
UARTController* getXBeeRadio();

#endif