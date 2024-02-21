#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"
#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "main.hpp"
#include "UART_config.hpp"

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
  ESP_LOGI(TAG, "starting UART config");

  ESP_LOGI(TAG, "UART configured successfully");

  //Begin I2C for accelerometer
  if(Wire.begin()) ESP_LOGI(TAG, "I2C Started Successfully");
  else ESP_LOGI(TAG, "I2C Failed");

  startTasks();
}

//runs in an infinite while loop (not really needed because of RTOS)
//Will not compile if removed
  void loop() {
    
  }