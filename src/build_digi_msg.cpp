#include "main.hpp"
#include "tasks.hpp"
#include "uart.hpp"
#include <string.h>
#include "build_digi_msg.hpp"

const char* DIGI_BUILD_TAG = "MSG_BULID";

Build_digi_msg::Build_digi_msg() {

}

uint8_t* Build_digi_msg::buildDigiMsg(uint8_t* rxAddress, uint8_t* txData, int txDataLen) {
    // A Transmit request frame always has 18 bytes of overhead; start delim (1), length (2), frame type (1), frame ID (1),
    // 64 bit addr (8), 16 bit addr (2), broadcast rad (1), tx options (1), checksum (1)
    const int TX_FRAME_OVERHEAD = 18;
    const int MSG_LEN_BYTES = 2;
    const int RX_ADDRESS_BYTES = 8;
    const int ADDR_SHORT_BYTES = 2;

    const uint8_t START_DELIM = 0x7E;
    const uint8_t FRAME_TYPE = 0x10;
    const uint8_t FRAME_ID = 0x01;
    const uint8_t ADDR_SHORT[ADDR_SHORT_BYTES] = {0xFF, 0xFE};
    const uint8_t BROADCAST_RAD = 0x00;
    const uint8_t TX_OPTIONS = 0x00;

    _txFrameSize = TX_FRAME_OVERHEAD + txDataLen;
    _txFrame = (uint8_t*) malloc(_txFrameSize+1);

    //_rfData = (uint8_t*) malloc(_dataSize+1);

    // Start building frame byte array
    int frameIdx = 0;
    _txFrame[frameIdx] = START_DELIM;
    frameIdx++;
    _txMsgLen(TX_FRAME_OVERHEAD, txDataLen);
    int lenOffset = frameIdx;

    for (int i=lenOffset; i<(MSG_LEN_BYTES+lenOffset); i++) {
        _txFrame[i] = _msgLen[i - lenOffset];                         // Should start at idx 0 and go to idx 1 (2 bytes)
        frameIdx++;
    }

    _txFrame[frameIdx] = FRAME_TYPE;
    frameIdx++;
    _txFrame[frameIdx] = FRAME_ID;
    frameIdx++;
    int rxAddrOffset = frameIdx;

    for(int x=rxAddrOffset; x<(RX_ADDRESS_BYTES+rxAddrOffset); x++) {
        _txFrame[x] = rxAddress[x - rxAddrOffset];
        frameIdx++;
    }
    int shortAddrOffset = frameIdx;

    for (int y=shortAddrOffset; y<(ADDR_SHORT_BYTES+shortAddrOffset); y++) {
        _txFrame[y] = ADDR_SHORT[y - shortAddrOffset];
        frameIdx++;
    }
    _txFrame[frameIdx] = BROADCAST_RAD;
    frameIdx++;
    _txFrame[frameIdx] = TX_OPTIONS;
    frameIdx++;
    int dataOffset = frameIdx;

    for (int z=dataOffset; z<(txDataLen+dataOffset); z++) {
        _txFrame[z] = txData[z - dataOffset];
        frameIdx++;
    }
    _txFrame[frameIdx] = _calcChecksum(_txFrame, _txFrameSize);

    ESP_LOG_BUFFER_HEXDUMP(DIGI_BUILD_TAG, _txFrame, _txFrameSize, ESP_LOG_INFO);
    return _txFrame;
}

void Build_digi_msg::_txMsgLen(int overheadBytes, int dataLen) {
    // The "msg length" of a frame is defined as the length of the frame excluding the start delimiter (1), length of the frame (2),
    // and the checksum (1) bytes
    const int MAX_BYTE_VALUE = 255;
    const int MSG_LEN_SIZE = 2;
    const int LEN_EXCLUDED_BYTES = 4;
    const int MSG_LEN_OVERHEAD = overheadBytes - LEN_EXCLUDED_BYTES;
    int numMsgBytes = MSG_LEN_OVERHEAD + dataLen; 

    if (numMsgBytes > MAX_BYTE_VALUE) {
        // Code given by Bing copilot
        for (int i = 0; i < MSG_LEN_SIZE; ++i) {
        _msgLen[i] = static_cast<uint8_t>((numMsgBytes >> (8 * i)) & 0xFF);
        }
    }
    else {
        _msgLen[0] = 0x00;
        _msgLen[1] = (uint8_t) numMsgBytes;
    }
    return;
}

uint8_t Build_digi_msg::_calcChecksum(uint8_t* frame, int frameLen) {
    /* This function will calculate the checksum for a Digimesh frame. */
    // Checksum calculation excludes start delim, and frame length (3 bytes) and checksum byte (pre-allocated in calling fnct)
    const int CHECKSUM_OFFSET = 3;
    // Dont want to add anything from the last idx since this is the spot for the checksum bits
    const int CHECKSUM_LENGTH = frameLen - 1;
    int checkSumIdx = CHECKSUM_OFFSET;
    // can just loop over frame at this point and add all values, then downcast to uint8
    int checkSum = 0;
    
    for(int i=CHECKSUM_OFFSET; i<CHECKSUM_LENGTH; i++) {
        checkSum += frame[i];
    }
    
    // Cast to uint8_t because we only care about the least significant 8 bits (defined in XBee manual)
    uint8_t checkSumResult = (uint8_t) checkSum; 
    // Checksum defined at result of 0xFF - checkSumResult (XBee manual)
    uint8_t checkSumByte = 0xFF - checkSumResult;
    
    return checkSumByte;
}

int Build_digi_msg::get_frameSize() {
    return _txFrameSize;
}

void Build_digi_msg::print_txFrame(uint8_t* frameBytes, int frameSize) {
    // Use for debugging
    ESP_LOG_BUFFER_HEXDUMP(DIGI_BUILD_TAG, frameBytes, frameSize, ESP_LOG_INFO);
}

char* Build_digi_msg::convertToAscii(int accData) {
    // Bing copilot gave example
    int sizeDataIn = sizeof(accData);
    _conversionArraySize = sizeDataIn;
    char charArray[sizeDataIn];
    ESP_LOGI(DIGI_BUILD_TAG, "DATA IN SIZE: %d", sizeDataIn);
    int iterStart = sizeDataIn -1;
    for(int i=iterStart; i >=0; --i) {
        charArray[i] = static_cast<char>((accData % 10) + '0');
        accData /= 10;
    }
    charArray[iterStart] = '\0'; // Null terminator
}

int Build_digi_msg::get_conversionArraySize() {
    return _conversionArraySize;
}

uint8_t* createDataArray(uint8_t* data, int lenData) {

}
