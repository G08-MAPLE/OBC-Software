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
  esp_log_level_set("*", ESP_LOG_INFO);
  ESP_LOGI(TAG, "Starting program");
  // startTasks();

}

//runs in an infinite while loop (not really needed because of RTOS)
void loop() {
  dartStateMachine.update();
}

