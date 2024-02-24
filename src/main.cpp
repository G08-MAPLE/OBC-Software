#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"
#include "main.hpp"
#include "sd_log.hpp"

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

#ifdef REASSIGN_PINS
    SPI.begin(sck, miso, mosi, cs);
#endif
    //if(!SD.begin(cs)){ //Change to this function to manually change CS pin
    if(!SD.begin()){
        ESP_LOGI(SD_TAG, "Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        ESP_LOGI(SD_TAG, "No SD card attached");
        return;
    }

    ESP_LOGI(SD_TAG, "SD Card Type: ");
    if(cardType == CARD_MMC){
        ESP_LOGI(SD_TAG, "MMC");
    } else if(cardType == CARD_SD){
        ESP_LOGI(SD_TAG, "SDSC");
    } else if(cardType == CARD_SDHC){
        ESP_LOGI(SD_TAG, "SDHC");
    } else {
        ESP_LOGI(SD_TAG, "UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    ESP_LOGI(SD_TAG, "SD Card Size: %lluMB\n", cardSize);

    writeFile(SD, "/output.txt", "");

  startTasks();
}

//runs in an infinite while loop (not really needed because of RTOS)
//Will not compile if removed
  void loop() {
    
  }