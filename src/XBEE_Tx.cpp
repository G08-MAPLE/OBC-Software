#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE //Defined before esp_log.h as per espressif docs

#include "esp_log.h"
#include "string.h"
#include "tasks.hpp"
#include "startup.hpp"
#include "build_digi_msg.hpp"
#include "XBEE_Tx.hpp"

/**
 * This thread will assume that the UART driver has already been configured.
 * It will be require access to the data to be transmitted. It should have
 * access to the XBEE_Rx thread so that Tx decisions can be made based off
 * successful data transfer.
 * Example code found: https://github.com/espressif/esp-idf/blob/v5.1.2/examples/peripherals/uart/uart_async_rxtxtasks/main/uart_async_rxtxtasks_main.c
*/
uint8_t GND_STATION_1_ADDR[8] = {0x00, 0x13, 0xA2, 0x00, 0x41, 0x5B, 0xAD, 0x65};
uint8_t GND_STATION_2_ADDR[8] = {0x00, 0x13, 0xA2, 0x00, 0x41, 0x5B, 0xAD, 0X6C};


void XBEE_tx(void * param){
    static const char *TX_TAG = "TX_TASK";
    UARTController* xBeeRadio = getXBeeRadio();             // Shares name across files to represent shared resource
    const int TX_DELAY = 1000;
    const int DATA_LENGTH = 10;

    for(;;){
        if (state == State::COMPLETE) {
            // TODO: Pull acc. data from SD card
            // TODO: Put acc. data into a Digimesh frame
            // TODO: Keep track of data in memory so that we know when all collected data has been transferred

            uint8_t dataTxHeader[] = {0x44, 0x61, 0x74, 0x61, 0x3A, 0x20};                  // "Data:_" in bytes

            int headerSize =sizeof(dataTxHeader);

            float dataTxData = 459.123456;                                 // TODO: Get data from SD card
            uint8_t* asciiData = parseAccToAscii(dataTxData);

            Build_digi_msg txMsg;
            uint8_t* dataIn = packageRFData(dataTxHeader, headerSize, asciiData, DATA_LENGTH);
            int newSize = headerSize + DATA_LENGTH;
            uint8_t* txFrame = txMsg.buildDigiMsg(GND_STATION_1_ADDR, dataIn, newSize);
            
            int frameSize = txMsg.get_frameSize();

            xBeeRadio -> XBEE_tx(txFrame, frameSize);
            vTaskDelay(pdMS_TO_TICKS(TX_DELAY));
        }
        else {
            vTaskDelay(pdMS_TO_TICKS(TX_DELAY));
        }
    }
}
uint8_t* parseAccToAscii(float accVal) {
    // Max acc value is 500.000 000 hence need a char array of 10 digits to represent value correctly
    const int MAX_DIGITS = 10;
    // Based on our ADC know that we have up to 6 decimal places (12-bit)
    const uint32_t ACC_OFFSET = 1000000;
    String TEST_TAG = "TEST";

    uint8_t* asciiAcc = (uint8_t*) malloc(sizeof(uint8_t)*10);
    float offsetAccValue = accVal*ACC_OFFSET;
    uint32_t accOffsetInt = (uint32_t) offsetAccValue;

    for (int a=0; a<MAX_DIGITS; a++) {
        uint8_t digit = accOffsetInt % 10;                  // Get least significant digit
        if ((MAX_DIGITS-a-1) == 3) {
            // At index three we know that we need a decimal point. Don't floor divide
            asciiAcc[MAX_DIGITS-a-1] = 0x2E;
        }
        else {
            asciiAcc[MAX_DIGITS-a-1] = '0' + digit;         // '0'+ value is ascii representation
            accOffsetInt = accOffsetInt / 10;               // Floor divide to get access to next digit
        }                            
        // Left for trouble shooting
        // ESP_LOGI(TEST_TAG, "Digit: %02X", asciiAcc[MAX_DIGITS-a-1]);
    }

    return asciiAcc;
}

uint8_t* packageRFData(uint8_t* headerArray, int headerLen, uint8_t* accAsciiVal, int valLen) {
    // TODO: Combine header and data into a single array so that it can be passed to the digimesh frame creator.
    String TEST_TAG = "TEST";
    uint8_t newSize = headerLen + valLen;
    uint8_t* comboArray = (uint8_t*) malloc(sizeof(uint8_t)*newSize);

    for(int i=0; i<headerLen; i++) {
        comboArray[i] = headerArray[i];
    }
    for(int x=0; x<valLen; x++) {
        comboArray[headerLen+x] = accAsciiVal[x];
    }

    // For troubleshooting
    // for(int y=0; y<newSize; y++) {
    //     ESP_LOGI(TEST_TAG,"%02X",comboArray[y]);
    // }
    return comboArray;
}