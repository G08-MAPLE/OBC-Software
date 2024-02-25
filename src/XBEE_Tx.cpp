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
            String asciiValue = (String) dataTxData;
            // itoa(dataTxData, dataString, 10);
            // ESP_LOGI(TX_TAG,"The Char array %0X", asciiValue);

            // uint8_t accLowByte = static_cast<uint8_t>((dataTxData & 0xFF00) >> 8);
            // ESP_LOGI(TX_TAG,"ACC LOW BYTE: %d", accLowByte);
            // char midConversion = (char) accLowByte;

            // uint8_t endConverison = (uint8_t) midConversion;
            // uint8_t accHighByte = static_cast<uint8_t>(dataTxData & 0x00FF);
            // ESP_LOGI(TX_TAG, "ACC HIGH BYTE: %d", accHighByte);

            // int testData = 500;
            // char* testInput;
            // testInput = itoa(testData, testInput, 10);
            // int inputSize = sizeof(testInput);

            // int testSize = 

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