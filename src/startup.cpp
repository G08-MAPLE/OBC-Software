#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "startup.hpp"

void startup(void * param){
    TickType_t blockOnceSecond = pdMS_TO_TICKS(1000);

    // UARTController xBeeRadio;
    // xBeeRadio.config();

    for(;;){
        if (state == State::BOOT) {
            // xBeeRadio.XBEE_tx();
            ESP_LOGI(START_TAG, "Changing States");
            if (xSemaphoreTake(stateMutex, ( TickType_t ) 100) == pdTRUE) {
                state = State::ARMED;
                ESP_LOGI(START_TAG, "State changed to ARMED");
                xSemaphoreGive(stateMutex);
            }
            else {
                ESP_LOGE(START_TAG, "Could not obtain mutex before timeout");
            }

        }

        else {
            vTaskDelay(blockOnceSecond);
            ESP_LOGI(START_TAG, "State is not in BOOT");
        }
    }
}