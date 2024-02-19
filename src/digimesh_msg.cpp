#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE //Defined before esp_log.h as per espressif docs

#include "digimesh_msg.hpp"

Digimesh_msg::Digimesh_msg(uint8_t* dataFrame){
    // No point in initializing instance variables on construction since they will be overwritten read methods
    _rawFrame = dataFrame;
}

int Digimesh_msg::digimesh_parse(uint8_t* dataFrame, int rxBuffIdx) {
    _extractStartDelimiter(dataFrame, rxBuffIdx);
    _extractMsgLength(dataFrame);
    _extractFrameType(dataFrame);
    if (_msgType == 0x90) {
        _extractSenderAddress(dataFrame);
        _extractShortAddress(dataFrame);
        _extractRxOption(dataFrame);
        _extractRfData(dataFrame);
        _extractChecksum(dataFrame);
        _verifyChecksum();
    }

    else if (_msgType == 0x89) {
        ESP_LOGI(DIGIMESH_TAG, "Transmit Status Received");
        _extractFrameId(dataFrame);
        _extractDeliveryStatus(dataFrame);
        _extractChecksum(dataFrame);
        _verifyChecksum();
    }

    else {
        ESP_LOGI(DIGIMESH_TAG, "Unexpected frame type received");
    }
    
    return _modBuffIdx;
}

void Digimesh_msg::_extractStartDelimiter(uint8_t* data, int buffIdx) {
    
    // Set original buffer index so that we know at what index of the Rx buffer we can find the start of the frame
    _ogBuffIdx = buffIdx;
    uint8_t _msgStartDelimiter = data[buffIdx];
    _modBuffIdx = buffIdx + 1;

    //Use for debugging
    ESP_LOGI(DIGIMESH_TAG, "Start Delimiter: %X", _msgStartDelimiter);
}

void Digimesh_msg::_extractMsgLength(uint8_t* data) {
    /* This function will extract the message length from a Digimesh frame. 
    It it important to note that this length refers to the length of the message excluding the; start delimiter (byte),
    length of message (2 bytes), and checksum (byte). */
    
    // Message length has maximum length of 2 bytes
    uint8_t msgLenBytes[2];

    for (int i=0; i<2; i++) {
        msgLenBytes[i] = data[_modBuffIdx];
        _modBuffIdx++;
    }
    //Byte x of a hexidecimal value is raised to the power of 16^(x-1)
    int msgLen = 16*msgLenBytes[0] + msgLenBytes[1];
    _msgLength = msgLen;

    // Use for trouble shooting
    ESP_LOGI(DIGIMESH_TAG, "Message Length Bytes: %02X %02X", msgLenBytes[0], msgLenBytes[1]);
    ESP_LOGI(DIGIMESH_TAG, "Length of received message: %d", _msgLength);
}

void Digimesh_msg::_extractFrameType(uint8_t* data) {
    /* This function will extract the frame type from the Digimesh frame. The frame type is always one byte long. */
    // Note: Should add lookup table later so that can convert from hex frame type value to human readable string frame type

    _msgType = data[_modBuffIdx];
    _modBuffIdx++; //Increment idx to point to next byte

    // Use for troubleshooting
    ESP_LOGI(DIGIMESH_TAG, "Frame Type: %X", _msgType);
}

void Digimesh_msg::_extractSenderAddress(uint8_t* dataFrame) {
    /* This function will extract the 64-bit sender address from the Digimesh frame. This is the serial number of the XBee
    module that sent the message. */

    // 64-bits is 8 bytes
    int maxSenderIdx = 8;

    for(int i=0; i<maxSenderIdx; i++) {
        _senderAddr[i] = dataFrame[_modBuffIdx];
        _modBuffIdx++;
    }

    ESP_LOGI(DIGIMESH_TAG, "Sender Address: %02X %02X %02X %02X %02X %02X %02X %02X", _senderAddr[0], _senderAddr[1],
             _senderAddr[2], _senderAddr[3], _senderAddr[4], _senderAddr[5], _senderAddr[6], _senderAddr[7]);
}

void Digimesh_msg::_extractShortAddress(uint8_t* dataFrame) {
    /* This function will extract the 16-bit address from the Digimesh frame. I'm not sure why/if this is useful */

    // 16-bits is 2 bytes
    int maxShortIdx = 2;

    for(int i=0; i<maxShortIdx; i++) {
        _shortAddr[i] = dataFrame[_modBuffIdx];
        _modBuffIdx++;
    }

    ESP_LOGI(DIGIMESH_TAG, "Short Address: %02X %02X", _shortAddr[0], _shortAddr[1]);
}

void Digimesh_msg::_extractRxOption(uint8_t* dataFrame) {
    /* This function will extract the receive option from the Digimesh frame. I'm not sure what this does. */
    _rxOptions = dataFrame[_modBuffIdx];
    _modBuffIdx++;

    ESP_LOGI(DIGIMESH_TAG, "Receive Option: %02X", _rxOptions);
}

void Digimesh_msg::_extractRfData(uint8_t* dataFrame) {
    /* This function will extract the data from the Digimesh frame. */

    // data(bytes) = msg size (bytes) - type (byte) - src addr (8 bytes) - sm. addr (2 bytes) - rx option (byte)
    // Simplifies to data(bytes) = msg size - 12 [bytes]
    const int overHeadBytes = 12;
    _dataSize = _msgLength - overHeadBytes;
    _rfData = (uint8_t*) malloc(_dataSize+1);

    for( int i=0; i<(_dataSize); i++) {
        _rfData[i] = dataFrame[_modBuffIdx];
        _modBuffIdx++;
    }

    // Use for debugging
    ESP_LOG_BUFFER_HEXDUMP(DIGIMESH_TAG, _rfData, _dataSize, ESP_LOG_INFO);
    /* Note this may need to be optimized if we are trying to maximize communication throughput. */
}

void Digimesh_msg::_extractChecksum(uint8_t* dataFrame) {
    /* This method will extract the checksum from a digimesh frame. */

    _msgChecksum = dataFrame[_modBuffIdx];
    _modBuffIdx++;

    // Use for debugging
    ESP_LOGI(DIGIMESH_TAG, "Checksum: %02X", _msgChecksum);
}

int Digimesh_msg::_verifyChecksum() {
    /* This function will verify data integrity of a frame by verifying its checksum. */
    int frameSum = 0;
    frameSum += _msgType;
    
    for (int i=0; i<_sizeSendAddr; i++) {
        frameSum += _senderAddr[i];
    }
    
    for (int x=0; x< _sizeShortAddr; x++) {
        frameSum += _shortAddr[x];
    }
    
    frameSum += _rxOptions;
    
    for (int y=0; y<_dataSize; y++) {
        frameSum += _rfData[y];
    }

    frameSum += _msgChecksum;
    // Cast to uint8_t because we only care about the least significant 8 bits (defined in XBee manual)
    frameSum = (uint8_t) frameSum; 

    if (frameSum == 0xFF) {
        // The sum of a correctly transmitted frame should be equal to 0xFF
        ESP_LOGI(DIGIMESH_TAG, "Frame Checksum is Correct.");
        return 1;
    }
    else {
        ESP_LOGI(DIGIMESH_TAG, "Frame Checksum is Incorrect.");
        ESP_LOGI(DIGIMESH_TAG, "Calculated Checksum: %X", frameSum);
        return -1;
    }
}

void Digimesh_msg::_extractFrameId(uint8_t* dataFrame) {
    /* This method will extract the Frame Id from the Digimesh frame. */
    _frameId = dataFrame[_modBuffIdx];
    _modBuffIdx++;

    ESP_LOGI(DIGIMESH_TAG, "Frame ID: %02X", _frameId);
}

void Digimesh_msg::_extractDeliveryStatus(uint8_t* dataFrame) {
    /* This method will extract the Delivery status from the Digimesh frame. */
    _deliveryStatus = dataFrame[_modBuffIdx];
    _modBuffIdx++;

    ESP_LOGI(DIGIMESH_TAG, "Delivery Status: %02X", _deliveryStatus);
}