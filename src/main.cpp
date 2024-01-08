#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE //Defined before esp_log.h as per espressif docs

#include <Arduino.h>
#include <stdio.h>
#include "main.hpp"
#include "UART_config.hpp"
#include "esp_log.h"

static const char *TAG = "MAIN";

//runs once on power up
void setup() {
  esp_log_level_set("*", ESP_LOG_INFO);
  ESP_LOGI(TAG, "starting UART config");

  UART_config();

  ESP_LOGI(TAG, "UART configured successfully");
  startTasks();
}

//runs in an infinite while loop (not really needed because of RTOS)
//Will not compile if removed
  void loop() {
    
  }


