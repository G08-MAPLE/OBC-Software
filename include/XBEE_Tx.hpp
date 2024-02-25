#ifndef XBEE_Tx_H
#define XBEE_Tx_H

#include <stdint.h>

void XBEE_tx(void * param);
char* parseAccToAscii(uint16_t accVal);
uint8_t* packageRFData(uint8_t* headerArray, int headerLen, uint8_t* accAsciiVal);

#endif