#ifndef spiffs_H
#define spiffs_H

#include <Arduino.h>
#include "esp_err.h"
#include "esp_spiffs.h"
#include "main.hpp"

static const char *SPIFFS_TAG = "FILE_SYS";

class Filesys {

public:
//TODO: Need to make better functions for reading and writing to an already existing file
//TODO: Need to make sure functions are thread safe
    Filesys();                                      //Class constructor esp_err_t
    void config();                                  //Set SPIFFS configuration
    
    //Above are config functions should be fine. Below will need to be updated
    void createFile();                              //Create a new file             TODO: params name of new file
    void findFile();                                //Check if file exists          TODO: params file to check?
    void renameFile();                              //Rename a file                 TODO: params file to rename, ____ ?
    void readFile();                                //Read from a file              TODO: params file to read, ____ ?
    void closeFileSys();                            //Close and disable SPIFFS

private:
    esp_vfs_spiffs_conf_t conf;                             //Let other class methods access config params, hide from rest of program
    void _partitionSizeCheck(esp_err_t partition);          //Check if partitions are valid sizes
    void _verifyPartitionSize(size_t sizeT, size_t usedT);  //Ensure partition sizes are consistent
};

#endif