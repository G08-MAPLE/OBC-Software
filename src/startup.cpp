#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "startup.hpp"

void startup(void * param){
    /* This task will run when the controller initially turns on. It will configure all peripherals needed
    for the project to operate.
    Required Peripherals(add as required):
        - Filesystem: For logging
        - Sensors (Accelerometer): For data collection
        - UART: To Rx commands and Tx data
        - I2C: Connection to sensors such as Accelerometer
    Once the peripherals have been configured state will be changed from BOOT
    to CONFIGURED and this task will suspend itself since it is only needed once per "life cycle".*/

    UARTController* xBeeRadio = getXBeeRadio();       //This object will need to be passed to other tasks in order to access UART? Global (extern)?
    // Filesys dartFs;                  //This object will also be needed in other tasks unless it becomes its own task? (still need config)
    // TODO: Create and initialize Accelerometer object this might just be the I2C bus
    
    for(;;){
        // !!!Must be paired with xTaskResumeAll() or else no other tasks will be able to run
        // Should probably just call startup function from main and do away with its task-ness
        if (state == State::BOOT) {
            // Initialize SPIFFS filesystem (fs)
            // dartFs.config();

            // Initialize Radio comms. (UART)
            xBeeRadio -> config();
            ESP_LOGI(START_TAG, "UART Controller created");
            
            // TODO: Initialize sensors and I2C bus 

            ESP_LOGI(START_TAG, "Changing States");
            if (xSemaphoreTake(stateMutex, ( TickType_t ) 100) == pdTRUE) {
                state = State::ONLINE;
                ESP_LOGI(START_TAG, "State changed to ONLINE");
                xSemaphoreGive(stateMutex);
            }
            else {
                ESP_LOGE(START_TAG, "Could not obtain mutex before timeout");
            }
        }

        else {
            ESP_LOGI(START_TAG, "State is not in BOOT");
            // If not in boot controller must be already configured. Suspend config task. Task will no longer be available to 
            // scheduler will need to call vTaskResume(startup) in order for this task to be accessible again.
            vTaskSuspend(NULL);     // passing NULL will suspend calling task
        }
    }
}