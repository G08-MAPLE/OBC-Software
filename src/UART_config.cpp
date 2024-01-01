#include "UART_config.hpp"
#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "../../../.platformio/packages/framework-espidf/components/soc/esp32/include/soc/clk_tree_defs.h"

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

#define UART_TXD_PIN 17 // IO17 on ESP-32
#define UART_RXD_PIN 16 // IO16 on ESP-32
#define UART_RTS_PIN (-1) // See sdkconfig.h or uart_echo_example
#define UART_CTS_PIN (-1)

//#define UART_PORT_NUM      2
#define UART_BAUD_RATE     115200
#define UART_STACK_SIZE    2048

static const char *TAG = "DART_UART";   // Don't think this is being used

#define BUF_SIZE (1024)

static void UART_config(void *arg){
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, UART_TXD_PIN, UART_RXD_PIN, UART_RTS_PIN, UART_CTS_PIN));

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
};