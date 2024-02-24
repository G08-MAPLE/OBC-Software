#include "XBEE_Rx.hpp"
#include "main.hpp"
#include "startup.hpp"

static UARTController xBeeRadio;
 
UARTController* getXBeeRadio(){
    return &xBeeRadio;
}

void XBEE_rx(void * param){
    const int RX_DELAY = 1000;                                    // Defines how often to read the Rx buffer
    const int RX_STATE_CHECK_DELAY = 1000;                        // Defines how often to check state in Rx task

    for(;;){
        if (state != State::BOOT && state != State::SLEEP) {     //Only plan to transfer data once test is complete
            // If new data in mem/files
            // Take mutex mem/file
            // Read data in from mem/file
            // If UART buffer not full
            // Release mutex

            xBeeRadio.XBEE_rx();
            vTaskDelay(pdMS_TO_TICKS(RX_DELAY));
        }

        else {
            vTaskDelay(pdMS_TO_TICKS(RX_STATE_CHECK_DELAY));    //Delay task
        }
    }
}