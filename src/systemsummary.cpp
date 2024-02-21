#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "systemsummary.hpp"

void systemSummary(void * param) {
    /* This task will run the systems checks on the dart so that we can verify that all the subsystems are ready to log during
    a test.
    Required tests: (Add as required)
        - Create a file
        - Read from sensor
        - Write sensor data to a file
        - Close file
        - Open file
        - Read file
        - Pass data from file to XBee for transmission
        - Once the data is ready to transmit this will trigger the heartbeat task to be created/called
        */
    
    int numChecks = 1;                      // Count the number of checks we expect to run successfully
    int passChecks = 0;
    for(;;) {
        if (state == State::CONFIGURED || state == State::COMPLETE ) {  //If state CONFIGURED or COMPLETE
            // Run system checks outlined above
            // Each check that runs should return 1 if successfull
            passChecks += testFileCreation();
            // If all checks pass state can be changed
            if (passChecks == numChecks) {
                // printSysSummary();
                // send summary to XBee for transmission
                // Once all data is in UART queue
                if (xSemaphoreTake(stateMutex, ( TickType_t ) 100) == pdTRUE) {     // Should make this semaphore structure a function it will be called often
                    if (state == State::CONFIGURED) {
                        state = State::ONLINE;
                        ESP_LOGI(SUMMARY_TAG, "State changed to ONLINE");
                    }
                    else if (state == State::CONFIGURED) {
                        state = State::SLEEP;
                        ESP_LOGI(SUMMARY_TAG, "State changed to SLEEP");
                        // Remember that SLEEP state should check that there is no more data left to send
                    }
                    else {
                        ESP_LOGI(SUMMARY_TAG, "Unrecognized entrance state");
                    }
                    xSemaphoreGive(stateMutex);
                }
                else {
                    ESP_LOGE(SUMMARY_TAG, "Could not obtain mutex before timeout");
                }
            }
            else {
                //Track number of tests that failed
                //printSysSummary
            }

        }
        else {
            ESP_LOGI(SUMMARY_TAG, "Current state not eligeable for system check");
            vTaskDelay(pdMS_TO_TICKS( 1000 ));
        }
    }
}
// Create a systemTest class that will contain all functions
// each test can have a variable that contains info about test status
// If all in a class will have access to this info from printSysSummary()
// Thus always call printSysSummary() whenever this task runs
int testFileCreation() { 
    //Create a test file
    // See spiffs.c for example
    // If file created successfully
    return 1;
} 

int testSensorRead() { 
    // Too generic if multiple sensors, but good for now
    // Try to read a value from the sensor. If data makes sense
    return 1;
}

int testFileWrite() { 
    //Test writing to file
    // see spiffs.c for example
    //Write to file, if successful
    return 1;
}

int testFileClose() { 
    //Close test file (combine with test write?)
    //if successful
    return 1;
}

int testFileOpen() { 
    //Open test file
    //if successful
    return 1;
}

int testFileRead() { 
    //Read from test file (combine with open file?)
    //If succeessful
    return 1;
}

int testXBeeUartLink() { 
    //Test XBee link
    //if successful
    return 1;
}

void printSysSummary() {
    /* Log test info to class variables during tests
    This function will print out the system status to a file/memory
    Should handle all cases both pass and fail*/
}