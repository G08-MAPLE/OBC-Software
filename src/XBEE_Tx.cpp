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

/**
 * This thread will assume that the UART driver has already been configured.
 * It will be require access to the data to be transmitted. It should have
 * access to the XBEE_Rx thread so that Tx decisions can be made based off
 * successful data transfer.
 * Example code found: https://github.com/espressif/esp-idf/blob/v5.1.2/examples/peripherals/uart/uart_async_rxtxtasks/main/uart_async_rxtxtasks_main.c
*/

int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_2, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

void XBEE_tx(void * param){
    static const char *TX_TAG = "TX_TASK";
    UARTController* xBeeRadio = getXBeeRadio();             // Shares name across files to represent shared resource
    const int TX_DELAY = 2000;

    for(;;){
        if (state == State::COMPLETE) {
            // TODO: Pull acc. data from SD card
            // TODO: Put acc. data into a Digimesh frame
            // TODO: Keep track of data in memory so that we know when all collected data has been transferred

            // sendData(TX_TAG, "DART to GND_CONTROL\n");
            xBeeRadio -> XBEE_digi_static_tx();
            vTaskDelay(pdMS_TO_TICKS(TX_DELAY));
            // vTaskDelay(4000 / portTICK_PERIOD_MS);          // This was the message sending speed of the example I copied

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