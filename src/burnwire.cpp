#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "burnwire.hpp"

void burn_manager(void * param){
    gpio_set_direction(PIN_BURN_WIRE, GPIO_MODE_OUTPUT);
    // Uncommenting following line is currently making this thread crash the controller.
    // ESP_LOGI(BURN_TAG, "Engaging Burn Thread");
    for(;;){
        if (state == State::LIVE) {
            gpio_set_level(PIN_BURN_WIRE, HIGH);            // Set Pin HIGH
            vTaskDelay(pdMS_TO_TICKS(3000));                // Delay to let wire burn
            gpio_set_level(PIN_BURN_WIRE, LOW);             // Set Pin LOW
            vTaskSuspend(NULL);                             // Suspend task since only need once
        }

        else {
            vTaskDelay(pdMS_TO_TICKS(1000));                // Delay if state not LIVE to save processing power
        }
    }
}