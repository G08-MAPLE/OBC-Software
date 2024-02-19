#ifndef DIGIMESH_MSG_H
#define DIGIMESH_MSG_H

#include "main.hpp"
#include "tasks.hpp"
#include "uart.hpp"
#include <string.h>

static const char *DIGIMESH_TAG = "DIGI_MSG";

class Digimesh_msg{
/* This class will contain all methods relating to parsing and building digimesh messages. */
private:
    // Variables
    uint8_t* _rawFrame;                                             // Pointer to the start of the dataframe
    
    int _msgStartDelimiter;                                         // Start delimiter of frame
    int _msgLength;                                                 // Length of Digimesh frame
    int _msgType;                                                   // Type of Digimesh frame
    uint8_t _senderAddr[8];                                         // 64-bit address of sender, 8 bytes (Serial Number)
    uint8_t _shortAddr[2];                                          // 16-bit address (I don't really know what this does yet)
    int _rxOptions;                                                 // Don't really know what this does either
    uint8_t* _rfData;                                               // Data embedded in the frame
    uint8_t _msgChecksum;                                           // Checksum sent with msg

    uint8_t _frameId;                                               // Frame ID for Tx'd message
    uint8_t _deliveryStatus;                                        // Delivery status of Tx'd message
    
    int _ogBuffIdx;                                                 // Original idx of the Rx buffer
    int _modBuffIdx;                                                // Modified version of the Rx buffer idx
    const int _sizeSendAddr = 8;                                    // Size in bytes of sender address array
    const int _sizeShortAddr = 2;                                   // Size in bytes of short address arry
    int _dataSize;                                                  // Size in bytes of data in frame

    // Methods
    void _extractStartDelimiter(uint8_t* data, int buffIdx);
    void _extractMsgLength(uint8_t* data);
    void _extractFrameType(uint8_t* data);
    void _extractSenderAddress(uint8_t* dataFrame);
    void _extractShortAddress(uint8_t* dataFrame);
    void _extractRxOption(uint8_t* dataFrame);
    void _extractRfData(uint8_t* dataFrame);
    void _extractChecksum(uint8_t* dataFrame);
    int _verifyChecksum();

    void _extractFrameId(uint8_t* dataFrame);
    void _extractDeliveryStatus(uint8_t* dataFrame);

public:
    // Digimesh_msg();                                                  // Constructor for msg creation
    Digimesh_msg(uint8_t* data);                                        // Constructor for msg reading
    int digimesh_parse(uint8_t* dataFrame, int rxBuffIdx);
};

#endif