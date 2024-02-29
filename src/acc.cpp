#include "acc.hpp"
#include <Arduino.h>
#include "ADC121C.hpp"
#include "esp_log.h"
#include "main.hpp"
#include <stdio.h>
#include <Wire.h>
#include "sd_log.hpp"

// using default i2c bus
// SDA = GPIO 21
// SCL = GPIO 22

String ACC_TAG = "ACC";

void acc_read(void * param){
    const int SAMPLING_RATE_DELAY = 10;               // Define the sampling rate based on delay vTaskDelayUntil() would provide more accurate sampling
    const int ACC_STATE_CHECK_DELAY = 100;            // Define how long to wait before re-checking system state
    unsigned int conversion;
    unsigned int measuredForce;
    char str[16];
    int i = 0;

    ADC121C mADC121C = ADC121C(0x50);                 // Address of ADC chip is 0x50, found experimentally
    mADC121C.begin();
    
    for(;;){
        if (state == State::ONLINE) {
            // conversion = mADC121C.readConversion();
            // measuredForce = gForceConversion(conversion);
            if(i < 10){
                measuredForce = 69;
                sprintf(str, "ACC: %d Gs\n", measuredForce);
                ESP_LOGI(ACC_TAG, "%s", str);
                sdLog(str);
                i++;
            }
            // ESP_LOGI(ACC_TAG, "Measured Force: %d G's", measuredForce);
            
            // Add support to write to SD card
            // Made sure that write process is thread safe

            vTaskDelay(pdMS_TO_TICKS(SAMPLING_RATE_DELAY));              // Run at ~100 Hz
        }
        else {
            vTaskDelay(pdMS_TO_TICKS(ACC_STATE_CHECK_DELAY));             // If state not LIVE wait before checking again
        }
    }
}

unsigned int gForceConversion(unsigned int rawADCValue) {
    // 12-bit ADC, max value = 4095
    // Max G Force is 500, and is proportional to value read from ADC
    const int maxGReading = 500;
    const int maxADCReading = 4095;
    unsigned int gForceValue = maxGReading*rawADCValue/maxADCReading;

    return gForceValue;
}