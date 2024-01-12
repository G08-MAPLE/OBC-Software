#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <Arduino.h>
#include <stdio.h>
#include <iostream>
#include "main.hpp"
#include "state_machine.hpp"
#include "esp_log.h"

static const char *TAG = "MAIN";

StateMachine dartStateMachine;

//runs once on power up
void setup() {
  //Create a time delay in Ticks
  TickType_t xTimeInTicks = pdMS_TO_TICKS(200);
  
  //Create Mutexes to ensure resources are used safely
  SemaphoreHandle_t stateMutex = xSemaphoreCreateMutex();       //State Mutex
  SemaphoreHandle_t flashMemMutex = xSemaphoreCreateMutex();    //Memory Mutex
  SemaphoreHandle_t memBufferMutex = xSemaphoreCreateMutex();   //Memory Buffer Mutex

  esp_log_level_set("*", ESP_LOG_INFO);
  ESP_LOGI(TAG, "Starting program");
  // xSemaphoreTake(stateMutex, xTimeInTicks);
  //   state = State::LIVE;
  //   ESP_LOGI(TAG, "Changing state in Mutex");
  // xSemaphoreGive(stateMutex);
  // ESP_LOGI(TAG, "%d", state);
  
  // startTasks();

}

//runs in an infinite while loop (not really needed because of RTOS)
void loop() {
  //dartStateMachine.update();
}

