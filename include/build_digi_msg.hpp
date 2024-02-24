#ifndef BUILD_DIGI_MSG_H
#define BUILD_DIGI_MSG_H

#include <stdint.h>

class Build_digi_msg{
private:
    int _txFrameSize;                                                   // Size of Frame to be transmitted
    uint8_t* _txFrame;
    uint8_t _msgLen[2];
    // Methods
    void _txMsgLen(int overheadBytes, int dataLen);                 // Will always be 2 bytes
    uint8_t _calcChecksum(uint8_t* frame, int frameLen);

public:
    Build_digi_msg();
    uint8_t* buildDigiMsg(uint8_t* rxAddress, uint8_t* txData, int txDataLen); // Always know rxAddress is 8 bits
    int get_frameSize();
    void print_txFrame(uint8_t* frameBytes, int frameSize);
};

#endif