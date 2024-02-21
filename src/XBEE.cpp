#include "XBEE.hpp"

static UARTController xBeeRadio;
 
UARTController* getXBeeRadio(){
    return &xBeeRadio;
}

void XBEE_rx(void * param){
    
    for(;;){
        if (state != State::BOOT && state != State::SLEEP) {     //Only plan to transfer data once test is complete
            // If new data in mem/files
            // Take mutex mem/file
            // Read data in from mem/file
            // If UART buffer not full
            // Release mutex

            xBeeRadio.XBEE_rx();
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        else {
            vTaskDelay(pdMS_TO_TICKS(1000));    //Delay task
        }
    }
}