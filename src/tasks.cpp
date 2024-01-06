#include "tasks.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void startTasks(){
    // xTaskCreate(
    //     startup,      // Function that should be called
    //     "startup",   // Name of the task (for debugging)
    //     1000,            // Stack size (bytes)
    //     NULL,            // Parameter to pass
    //     1,               // Task priority
    //     NULL             // Task handle
    // );

    xTaskCreate(
        XBEE_rx,      // Function that should be called
        "XBEE_rx",   // Name of the task (for debugging)
        1024*4,            // Stack size (bytes) (MAY NEED MORE BASED ON BUFFER SIZE)
        NULL,            // Parameter to pass
        configMAX_PRIORITIES,               // Task priority
        NULL             // Task handle
    );

    xTaskCreate(
        XBEE_tx,      // Function that should be called
        "XBEE_tx",   // Name of the task (for debugging)
        1024*2,            // Stack size (bytes) (MAY NEED MORE BASED ON BUFFER SIZE)
        NULL,            // Parameter to pass
        configMAX_PRIORITIES-1,               // Task priority
        NULL             // Task handle
    );

    // xTaskCreate(
    //     heartbeat,      // Function that should be called
    //     "heartbeat",   // Name of the task (for debugging)
    //     1000,            // Stack size (bytes)
    //     NULL,            // Parameter to pass
    //     1,               // Task priority
    //     NULL             // Task handle
    // );

    // xTaskCreate(
    //     burn_manager,      // Function that should be called
    //     "burn manager",   // Name of the task (for debugging)
    //     1000,            // Stack size (bytes)
    //     NULL,            // Parameter to pass
    //     1,               // Task priority
    //     NULL             // Task handle
    // );

    // xTaskCreate(
    //     acc_read,      // Function that should be called
    //     "acclerometer",   // Name of the task (for debugging)
    //     1000,            // Stack size (bytes)
    //     NULL,            // Parameter to pass
    //     1,               // Task priority
    //     NULL             // Task handle
    // );
}