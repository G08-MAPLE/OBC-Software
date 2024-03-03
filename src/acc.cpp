#include "acc.hpp"
#include <Arduino.h>
#include "ADC121C.hpp"
#include "esp_log.h"
#include "main.hpp"
#include <stdio.h>
#include <Wire.h>

// using default i2c bus
// SDA = GPIO 21
// SCL = GPIO 22

String ACC_TAG = "ACC";

void acc_read(void * param){
    const int SAMPLING_RATE_DELAY = 10;               // Define the sampling rate based on delay vTaskDelayUntil() would provide more accurate sampling
    const int ACC_STATE_CHECK_DELAY = 100;            // Define how long to wait before re-checking system state
    unsigned int conversion;
    float measuredForce;

    ADC121C mADC121C = ADC121C(0x50);                 // Address of ADC chip is 0x50, found experimentally, all chips the same
    mADC121C.begin();
    
    for(;;){
        if (state == State::LIVE) {
            conversion = mADC121C.readConversion();
            measuredForce = gForceConversion(conversion);
            ESP_LOGI(ACC_TAG, "Measured Force: %f G's", measuredForce);
            
            // Add support to write to SD card
            // Made sure that write process is thread safe

            vTaskDelay(pdMS_TO_TICKS(SAMPLING_RATE_DELAY));              // Run at ~100 Hz
        }
        else {
            vTaskDelay(pdMS_TO_TICKS(ACC_STATE_CHECK_DELAY));             // If state not LIVE wait before checking again
        }
    }
}

float gForceConversion(unsigned int rawADCValue) {
    // 12-bit ADC, max value = 4095
    // Max G Force is 500, and is proportional to value read from ADC
    // Float should have 7 decimal values by default
    const int maxGReading = 500;
    const int maxADCReading = 4095;
    // Float conversion needed to get an actual float value
    float gForceValue = (float) maxGReading*rawADCValue/maxADCReading;

    return gForceValue;
}