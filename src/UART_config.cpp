#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE //Defined before esp_log.h as per espressif docs

#include "UART_config.hpp"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include <stdio.h>


/**
 * This task will send the data collected by the board to the XBEE for wireless transfer using UART,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: configured UART
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below (See Kconfig)
 */

#define UART_TXD_PIN (GPIO_NUM_17) // IO17 on ESP-32
#define UART_RXD_PIN (GPIO_NUM_16) // IO16 on ESP-32
#define UART_RTS_PIN (-1) // See sdkconfig.h or uart_echo_example
#define UART_CTS_PIN (-1)

//#define UART_PORT_NUM      2
#define UART_BAUD_RATE     115200
#define UART_STACK_SIZE    2048

static const char *UART_TAG = "UART_CONFIG";
static const int RX_BUF_SIZE = 1024;
// #define BUF_SIZE (1024)

void UART_config(void){
    esp_log_level_set(UART_TAG, ESP_LOG_INFO);
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,       //Based on https://github.com/espressif/esp-idf/blob/v4.2/examples/peripherals/uart/uart_async_rxtxtasks/main/uart_async_rxtxtasks_main.c
    };

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_LOGI(UART_TAG, "Driver Installed");
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
    ESP_LOGI(UART_TAG, "Parameters Configured");
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, UART_TXD_PIN, UART_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_LOGI(UART_TAG, "Pins Configured");
};