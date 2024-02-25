#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE //Defined before esp_log.h as per espressif docs

#include "XBEE_Tx.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "tasks.hpp"
#include "startup.hpp"
#include "build_digi_msg.hpp"
#include <cstring>
using namespace std;

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

    for(;;){
        if (state == State::COMPLETE) {
            // TODO: Pull acc. data from SD card
            // TODO: Put acc. data into a Digimesh frame
            // TODO: Keep track of data in memory so that we know when all collected data has been transferred

            uint8_t dataTxHeader[] = {0x44, 0x61, 0x74, 0x61, 0x3A, 0x20};                  // "Data:_" in bytes
            int headerSize =sizeof(dataTxHeader);
            uint16_t dataTxData = 459;
            parseAccToAscii(dataTxData);

            // uint8_t tx_Test[] = {0x44, 0x61, 0x74, 0x61, 0x3A, 0x20, 0x33, 0x30, 0x30};
            // int testLen = sizeof(tx_Test);
            // ESP_LOGI(TX_TAG,"CALCULATED SIZE OF ARRAY: %d", testLen);

            // char* dataIn = "Data: 300";

            xBeeRadio -> XBEE_digi_static_tx(0);
            // Build_digi_msg txMsg;
            // uint8_t* txFrame = txMsg.buildDigiMsg(GND_STATION_1_ADDR, dataIn, newSize);
            
            // int frameSize = txMsg.get_frameSize();

            // xBeeRadio -> XBEE_tx(txFrame, frameSize);
            //vTaskDelay(pdMS_TO_TICKS(TX_DELAY));
            vTaskSuspend(NULL);

            // TODO: IF all data has been transferred
            // if (xSemaphoreTake(stateMutex, ( TickType_t ) 100) == pdTRUE) {
            //             if (state == State::COMPLETE) {
            //                 state = State::SLEEP;
            //                 ESP_LOGI(TX_TAG, "State changed to SLEEP");
            //             }
            //             xSemaphoreGive(stateMutex);
            //         }
            //         else {
            //             ESP_LOGE(TX_TAG, "Could not obtain mutex before timeout");
            //         }
        }
        else {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

char* parseAccToAscii(uint16_t accVal) {
    // Max acc value is 500 hence need a char array of 3 digits to represent value correctly
    String TEST_TAG = "TEST";
    char asciiAcc[3] = {0x00, 0x00, 0x00};
    uint8_t oneDigit = accVal % 10;                     //Get digit in 1's place
    ESP_LOGI(TEST_TAG, "One's place: %d", oneDigit);
    accVal = accVal / 10;                               // Floor divide for access to 10's digit
    uint8_t tenDigit = accVal % 10;
    ESP_LOGI(TEST_TAG, "Ten's place: %d", tenDigit);
    accVal = accVal / 10;
    uint8_t hundredDigit = accVal % 10;
    ESP_LOGI(TEST_TAG, "Hundred's place: %d", hundredDigit);
    asciiAcc[2] = (char) '0' + oneDigit;
    asciiAcc[1] = (char) '0' + tenDigit;
    asciiAcc[0] = (char) '0' + hundredDigit;
    ESP_LOGI(TEST_TAG, "Attempted ASCII string: %02X %02X %02X", (uint8_t) asciiAcc[0], (uint8_t) asciiAcc[1], asciiAcc[2]);
    
    return asciiAcc;
}

uint8_t* packageRFData(uint8_t* headerArray, int headerLen, uint8_t* accAsciiVal) {
    // TODO: Combine header and data into a single array so that it can be passed to the digimesh frame creator.
}