#include "tasks.hpp"
#include "main.hpp"

void startTasks(){
    xTaskCreate(
        startup,      // Function that should be called
        "startup",   // Name of the task (for debugging)
        2000,            // Stack size (bytes)
        NULL,            // Parameter to pass
        1,               // Task priority
        NULL             // Task handle
    );
    
    // xTaskCreate(
    //     XBEE_rx,      // Function that should be called
    //     "XBEE rx",   // Name of the task (for debugging)
    //     2000,            // Stack size (bytes) (MAY NEED MORE BASED ON BUFFER SIZE)
    //     NULL,            // Parameter to pass
    //     1,               // Task priority
    //     NULL             // Task handle
    // );

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
