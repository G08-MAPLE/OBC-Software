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
    unsigned int measuredForce;

    for(;;){
        conversion = mADC121C.readConversion();
        measuredForce = gForceConversion(conversion);
        ESP_LOGI(ACC_TAG, "Measured Force: %d G's\n", measuredForce);
        
        // Add support to write to SD card
        // Made sure that write process is thread safe

        vTaskDelay(pdMS_TO_TICKS(10));              // Run at ~100 Hz
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