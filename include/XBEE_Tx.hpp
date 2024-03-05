#ifndef XBEE_Tx_H
#define XBEE_Tx_H

#include <stdint.h>

void XBEE_tx(void * param);
uint8_t* parseAccToAscii(float accVal);
uint8_t* packageRFData(uint8_t* headerArray, int headerLen, uint8_t* accAsciiVal, int valLen);

#endif