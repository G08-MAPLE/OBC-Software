#ifndef XBEE_Rx_H
#define XBEE_Rx_H

#include "uart.hpp"                 // Required to have UARTController type in header

void XBEE_rx(void * param);
UARTController* getXBeeRadio();

#endif