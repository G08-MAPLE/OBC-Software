#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "burnwire.hpp"

void burn_manager(void * param){
    /* This thread will activate the burn wire for a predetermined duration before deactivating it. The task will then
    suspend itself since the burn wire can only be used once per test cycle. The delay of the burn wire must be found 
    through physical testing. The required delay will change if any alterations are made to the physical burn wire.*/

    gpio_set_direction(PIN_BURN_WIRE, GPIO_MODE_OUTPUT);    // Set Pin 32 as a digital output
    // Uncommenting following line is currently making this thread crash the controller.
    // ESP_LOGI(BURN_TAG, "Engaging Burn Thread");
    for(;;){
        if (state == State::LIVE) {
            gpio_set_level(PIN_BURN_WIRE, HIGH);            // Set Pin HIGH
            vTaskDelay(pdMS_TO_TICKS(15000));                // Delay to let wire burn
            gpio_set_level(PIN_BURN_WIRE, LOW);             // Set Pin LOW
            // vTaskDelay(pdMS_TO_TICKS(5000));                // Delay to let wire burn
            vTaskSuspend(NULL);                             // Suspend task since only need once
        }

        else {
            vTaskDelay(pdMS_TO_TICKS(5000));                // Delay if state not LIVE to save processing power
        }
    }
}