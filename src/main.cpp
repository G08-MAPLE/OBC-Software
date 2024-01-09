#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <Arduino.h>
#include <stdio.h>
#include <iostream>
#include "main.hpp"
#include "state_machine.hpp"
#include "esp_log.h"
using namespace std;

static const char *TAG = "MAIN";

enum class Test {
  Powered,
  SystemOnline,
  Live,
};

void (*state)();

// ----------------
// function prototypes
// ----------------
void poweredState();
void systemOnlineState();
void liveState();

void poweredState() {
  ESP_LOGI(TAG, "Option 1 confirmed");
  state = systemOnlineState;
}

void systemOnlineState() {
  ESP_LOGI(TAG, "Option 2 confirmed");
  state = liveState;
}

void liveState() {
  ESP_LOGI(TAG, "Test successful");
}

StateMachine dartStateMachine;
//runs once on power up
void setup() {
  esp_log_level_set("*", ESP_LOG_INFO);
  // ESP_LOGI(TAG, "Starting program");
  // //startTasks();
  // Test enumValue = Test::Powered;
  // ESP_LOGI(TAG, "enumValue: %d", enumValue);
  
  
  //dartStateMachine.StateMachine::state();
  //ESP_LOGI(TAG, dartStateMachine.state);
  //dartStateMachine.state;
  // state = poweredState;
  // state();

}

//runs in an infinite while loop (not really needed because of RTOS)
void loop() {
  dartStateMachine.update();
}

