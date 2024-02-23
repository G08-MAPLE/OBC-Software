#ifndef ACC_H
#define ACC_H

#include <Arduino.h>
#include "ADC121C.hpp"
#include "esp_log.h"
#include "main.hpp"
#include <stdio.h>
#include <Wire.h>

void acc_read(void * param);
unsigned int gForceConversion(unsigned int rawADCValue);

#endif