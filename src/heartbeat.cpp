#include <Arduino.h>
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "heartbeat.hpp"

static conn_state_t xbee_state;
static int misses;

int get_xbee_state(){
    return xbee_state;
}

int get_misses(){
    return misses;
}

void heartbeat(void * param){
    BaseType_t retRTOS;
    xbee_state = CONN_NONE;
    misses = 0;

    for(;;){
		//Check if ground computer has sent a message
		retRTOS = xTaskNotifyWait(0x00,0x00, NULL, HEARTBEAT_TASK_DELAY_MS);

		//check if the ground station responded
		if(retRTOS == pdTRUE){
			//received notification

			//reset misses counter
			misses = 0;

			//check if we have previously lost the connection
			if(xbee_state == CONN_LOST){
                //Log(XBEE: Reconnected)
			}
			else if(xbee_state == CONN_NONE){
				//connecting with ground station for the first time
                //Log(XBEE: Made connection with ground station)
			}

			//set state
			xbee_state = CONN_PRESENT;
		}
		else{
			//did not receive notification from ground computer
			if(xbee_state == CONN_PRESENT){

				if(misses < MAX_MISSES){
					//losing ground computer
					misses++;

					//just for safety
					xbee_state = CONN_PRESENT;
				}
				else{
					//lost ground computer
					xbee_state = CONN_LOST;
					//Log(XBEE: Lost connection to the ground computer)
				}
			}
			else if(xbee_state == CONN_NONE ){
				if(misses < MAX_MISSES){
					//losing ground computer
					misses++;

					//just for safety
					xbee_state = CONN_NONE;

				}
				else{
					//lost ground computer
					xbee_state = CONN_NONE;
					//Log(could not connect to ground computer)
				}
			}
			else{
				
			}
		}
    }
}