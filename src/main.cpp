#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"
#include "main.hpp"

static const char *TAG = "MAIN";

//Create Mutexes to ensure resources are used safely
SemaphoreHandle_t stateMutex = xSemaphoreCreateMutex();       //State Mutex
SemaphoreHandle_t flashMemMutex = xSemaphoreCreateMutex();    //Memory Mutex
SemaphoreHandle_t memBufferMutex = xSemaphoreCreateMutex();   //Memory Buffer Mutex

//Initialize state
State state = State::BOOT;

//runs once on power up
void setup() {
  
  esp_log_level_set("*", ESP_LOG_INFO);
  ESP_LOGI(TAG, "Starting program");

  startTasks();
}

//runs in an infinite while loop (not really needed because of RTOS)
//Will not compile if removed
  void loop() {
    
  }