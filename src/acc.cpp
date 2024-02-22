#include <Arduino.h>
#include "acc.hpp"
#include <Wire.h>
#include "ADC121C.hpp"
#include "esp_log.h"
#include <stdio.h>

// using default i2c bus
// SDA = GPIO 21
// SCL = GPIO 22

String ACC_TAG = "ACC";

void acc_read(void * param){
    ADC121C mADC121C = ADC121C(0x50);               // Address of ADC chip is 0x50, found experimentally
    mADC121C.begin();
    unsigned int conversion;

    for(;;){
        conversion = mADC121C.readConversion();
        ESP_LOGI(ACC_TAG, "ADC: %d\n", conversion);
        
        // Add support to write to SD card
        // Made sure that write process is thread safe

        vTaskDelay(pdMS_TO_TICKS(10));              // Run at ~100 Hz
    }
}