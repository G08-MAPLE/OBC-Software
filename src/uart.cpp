#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE //Defined before esp_log.h as per espressif docs

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "pindefs.hpp"
#include "string.h"
#include "uart.hpp"

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

void UARTController::XBEE_tx() {
        _sendData(UART_TAG, "DART to GND_CONTROL\n");
        // vTaskDelay(4000 / portTICK_PERIOD_MS);                      //This is default message rate
        // TODO look into messaging rates to maximized data collection
}