#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE //Defined before esp_log.h as per espressif docs

#include "digimesh_msg.hpp"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "pindefs.hpp"
#include "string.h"
#include "uart.hpp"

// #include <iostream>

/**
 * This class will contain all of the functions necessary to create a UART driver required to interface with an
 * XBee radio. Hardware flow control is turned off. It does not use UART driver event queue.
 *
 * - Port: configured UART
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below (See Kconfig)
 */

#define UART_TXD_PIN (PIN_XBEE_TX) // GPIO_NUM_17  IO17 on ESP-32
#define UART_RXD_PIN (PIN_XBEE_RX) // GPIO_NUM_16  IO16 on ESP-32

#define UART_BAUD_RATE     115200
#define UART_STACK_SIZE    2048
// #define RX_BUF_SIZE        (1024)
static const int RX_BUF_SIZE = 1024;


UARTController::UARTController(){
    // const int RX_BUF_SIZE = 1024;
};

void UARTController::config(void){
    //This function will configure the ESP32's UART2 port
    //Would be better practice to parameterize this, but our project only uses a single UART port so not worth our time

    esp_log_level_set(UART_TAG, ESP_LOG_INFO);
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,       
        //Based on https://github.com/espressif/esp-idf/blob/v4.2/examples/peripherals/uart/uart_async_rxtxtasks/main/uart_async_rxtxtasks_main.c
    };

    try {
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, RX_BUF_SIZE * 2, 0, 0, NULL, 0));   //
        ESP_LOGI(UART_TAG, "Driver Installed");
        ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
        ESP_LOGI(UART_TAG, "Parameters Configured");
        ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, UART_TXD_PIN, UART_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        ESP_LOGI(UART_TAG, "Pins Configured");
    }

    catch (esp_err_t paramError) {
        ESP_LOGE(UART_TAG, "UART configuration failed: Check paramaters");
    }
};

// The following two functions have been taken from the ESP-IDF example pages
// Example code found: https://github.com/espressif/esp-idf/blob/v5.1.2/examples/peripherals/uart/uart_async_rxtxtasks/main/uart_async_rxtxtasks_main.c

int UARTController::_sendData(const char* logName, const char* data) {
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_2, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

void UARTController::XBEE_tx(const char* dataTx) {
        _sendData(UART_TAG, dataTx);
        // vTaskDelay(4000 / portTICK_PERIOD_MS);                      //This is default message rate
        // TODO look into messaging rates to maximized data collection
}

void UARTController::XBEE_digi_tx() {
    uint16_t hex_data[] = {0x7E, 0x00, 0x1E, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x5B, 0xAD, 0x65, 0xFF, 0xFE, 0x00, 0x00,
                          0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x66, 0x72, 0x6F, 0x6D, 0x20, 0x44, 0x41, 0x52, 0x54, 0x21, 0x5A};

    uint16_t hex_data1[] = {0x5A, 0x21, 0x54, 0x52, 0x41, 0x44, 0x20, 0x6D, 0x6F, 0x72, 0x66, 0x20, 0x6F, 0x6C, 0x6C, 0x65, 0x48,
                           0x00, 0x00, 0xFE, 0xFF, 0x65, 0xAD, 0x5B, 0x41, 0x00, 0xA2, 0x13, 0x00, 0x01, 0x10, 0x1E, 0x00, 0x7E};

    int data_len = sizeof(hex_data1);
    const int txBytes = uart_write_bytes(UART_NUM_2, hex_data1, data_len);
    ESP_LOGI(UART_TAG, "Wrote %d bytes", txBytes);
}

void UARTController::XBEE_rx() {
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    const int rxBytes = uart_read_bytes(UART_NUM_2, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(UART_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(UART_TAG, data, rxBytes, ESP_LOG_INFO);
            // Parse incoming data
            _parseData(data);
        }
    free(data); // This was in original file that came from example code, think it has something to do with clearing buffer should look into in the future
}

void UARTController::_parseData(uint8_t* data) {
    int buffIdx = 0;
    uint8_t startDelimiter = data[0];   // Use uint8_t since it matches example which is working
    
    if (startDelimiter == 0x7E) {       // The start delimiter of a Digimesh message is always 7E
        Digimesh_msg currentMsg = Digimesh_msg(data);
        currentMsg.digimesh_parse(data, buffIdx);
        // Depending on message call different functions
        // Increment buffIdx based on bytes read

        // want to clear the bytes from the buffer once they have been read
    }

    else if (startDelimiter == 0) { 
        // End of buffer end function???
    }

    else {
        buffIdx++; //Increase buffer index to try to find next message
    }
   
}