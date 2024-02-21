#ifndef SYSTEMSUMMARY_H
#define SYSTEMSUMMARY_H

#include "main.hpp"
#include "tasks.hpp"
#include "spiffs.hpp"
#include "uart.hpp"

static const char *SUMMARY_TAG = "Summary";

void systemSummary(void * param);

int testFileCreation();     //Create a test file
int testSensorRead();       // Too generic if multiple sensors, but good for now
int testFileWrite();        //Test writing to file
int testFileClose();        //Close test file (combine with test write?)
int testFileOpen();         //Open test file
int testFileRead();         //Read from test file (combine with open file?)
int testXBeeUartLink();     //Test XBee link
void printSysSummary();     //Write system summary to a file so that it can be read

#endif