#include "XBEE.hpp"

void XBEE_rx(void * param){
    
    for(;;){
        if (state == State::COMPLETE) {     //Only plan to transfer data once test is complete
            // If new data in mem/files
            // Take mutex mem/file
            // Read data in from mem/file
            // If UART buffer not full
            // xBeeRadio.XBEE_tx();            //Need to modify XBEE_tx() so that it accepts data as a parameter
            // Release mutex
        }

        else {
            vTaskDelay(pdMS_TO_TICKS(1000));    //Delay task
        }
    }
}