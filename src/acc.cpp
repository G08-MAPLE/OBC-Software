#include <Arduino.h>
#include "acc.hpp"
#include <Wire.h>
#include "ADC121C.hpp"
#include "esp_log.h"
#include <stdio.h>

// using default i2c bus
// SDA = GPIO 21
// SCL = GPIO 22

char * TAG = "ACC";

void acc_read(void * param){
    ADC121C mADC121C = ADC121C(0x55);
    unsigned int conversion;

    for(;;){
        conversion = mADC121C.readConversion();
        ESP_LOGI(TAG, "ADC: %d\n", conversion);
    }
}