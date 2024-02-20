#include "config.h"
#include "tasks.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void startTasks(){

#if configENABLE_STARTUP_TASK > 0
    xTaskCreate(
        startup,      // Function that should be called
        "startup",   // Name of the task (for debugging)
        1000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );
#endif

#if configENABLE_XBEE_RX_TASK > 0
    xTaskCreate(
        XBEE_rx,      // Function that should be called
        "XBEE_rx",   // Name of the task (for debugging)
        1024*4,            // Stack size (bytes) (MAY NEED MORE BASED ON BUFFER SIZE)
        NULL,            // Parameter to pass
        configMAX_PRIORITIES,               // Task priority
        NULL             // Task handle
    );
#endif

#if configENABLE_XBEE_TX_TASK > 0
    xTaskCreate(
        XBEE_tx,      // Function that should be called
        "XBEE_tx",   // Name of the task (for debugging)
        1024*2,            // Stack size (bytes) (MAY NEED MORE BASED ON BUFFER SIZE)
        NULL,            // Parameter to pass
        configMAX_PRIORITIES-1,               // Task priority
        NULL             // Task handle
    );
#endif

#if configENABLE_HEARTBEAT_TASK > 0
    xTaskCreate(
        heartbeat,      // Function that should be called
        "heartbeat",   // Name of the task (for debugging)
        1000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );
#endif

#if configENABLE_BURNWIRE_TASK > 0
    xTaskCreate(
        burn_manager,      // Function that should be called
        "burn manager",   // Name of the task (for debugging)
        1000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );
#endif

#if configENABLE_ACCELEROMETER_TASK > 0
    xTaskCreate(
        acc_read,      // Function that should be called
        "acclerometer",   // Name of the task (for debugging)
        1000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );
#endif
}