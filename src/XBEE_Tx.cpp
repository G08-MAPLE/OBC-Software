#include "XBEE_Tx.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

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

static void XBEE_tx(void * param){
    
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    for(;;){
        sendData(TX_TASK_TAG, "Hello world");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}