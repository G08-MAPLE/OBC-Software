/*
 * pin 1 - not used          |  Micro SD card     |
 * pin 2 - CS (SS)           |                   /
 * pin 3 - DI (MOSI)         |                  |__
 * pin 4 - VDD (3.3V)        |                    |
 * pin 5 - SCK (SCLK)        | 8 7 6 5 4 3 2 1   /
 * pin 6 - VSS (GND)         | ▄ ▄ ▄ ▄ ▄ ▄ ▄ ▄  /
 * pin 7 - DO (MISO)         | ▀ ▀ █ ▀ █ ▀ ▀ ▀ |
 * pin 8 - not used          |_________________|
 *                             ║ ║ ║ ║ ║ ║ ║ ║
 *                     ╔═══════╝ ║ ║ ║ ║ ║ ║ ╚═════════╗
 *                     ║         ║ ║ ║ ║ ║ ╚══════╗    ║
 *                     ║   ╔═════╝ ║ ║ ║ ╚═════╗  ║    ║
 * Connections for     ║   ║   ╔═══╩═║═║═══╗   ║  ║    ║
 * full-sized          ║   ║   ║   ╔═╝ ║   ║   ║  ║    ║
 * SD card             ║   ║   ║   ║   ║   ║   ║  ║    ║
 * Pin name         |  -  DO  VSS SCK VDD VSS DI CS    -  |
 * SD pin number    |  8   7   6   5   4   3   2   1   9 /
 *                  |                                  █/
 *                  |__▍___▊___█___█___█___█___█___█___/
 *
 * Note:  The SPI pins can be manually configured by using `SPI.begin(sck, miso, mosi, cs).`
 *        Alternatively, you can change the CS pin and use the other default settings by using `SD.begin(cs)`.
 *
 * +--------------+---------+-------+----------+----------+----------+
 * | SPI Pin Name | ESP8266 | ESP32 | ESP32-S2 | ESP32-C3 | ESP32-S3 |
 * +==============+=========+=======+==========+==========+==========+
 * | CS (SS)      | GPIO15  | GPIO5 | GPIO5    | GPIO13   | GPIO13   |
 * +--------------+---------+-------+----------+----------+----------+
 * | DI (MOSI)    | GPIO13  | GPIO23| GPIO24   | GPIO14   | GPIO14   |
 * +--------------+---------+-------+----------+----------+----------+
 * | DO (MISO)    | GPIO12  | GPIO19| GPIO25   | GPIO15   | GPIO15   |
 * +--------------+---------+-------+----------+----------+----------+
 * | SCK (SCLK)   | GPIO14  | GPIO18| GPIO19   | GPIO16   | GPIO16   |
 * +--------------+---------+-------+----------+----------+----------+
 *
 * For more info see file README.md in this library or on URL:
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
 */


#include "sd_log.hpp"
#include "esp_log.h"

/*
Uncomment and set up if you want to use custom pins for the SPI communication
#define REASSIGN_PINS
int sck = -1;
int miso = -1;
int mosi = -1;
int cs = -1;
*/

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    ESP_LOGI(SD_TAG, "Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        ESP_LOGI(SD_TAG, "Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        ESP_LOGI(SD_TAG, "Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            ESP_LOGI(SD_TAG, "  DIR : ");
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            ESP_LOGI(SD_TAG, "  FILE: %s, SIZE: %d", file.name(), file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    ESP_LOGI(SD_TAG, "Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        ESP_LOGI(SD_TAG, "Dir created");
    } else {
        ESP_LOGI(SD_TAG, "mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    ESP_LOGI(SD_TAG, "Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        ESP_LOGI(SD_TAG, "Dir removed");
    } else {
        ESP_LOGI(SD_TAG, "rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    ESP_LOGI(SD_TAG, "Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        ESP_LOGI(SD_TAG, "Failed to open file for reading");
        return;
    }

    ESP_LOGI(SD_TAG, "Read from file: ");
    while(file.available()){
        ESP_LOGI(SD_TAG, "%s", file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        ESP_LOGI(SD_TAG, "Failed to open file for writing");
        return;
    }
    if(!file.print(message)){
        ESP_LOGI(SD_TAG, "Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        ESP_LOGI(SD_TAG, "Failed to open file for appending");
        return;
    }
    if(!file.print(message)){
        ESP_LOGI(SD_TAG, "Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    ESP_LOGI(SD_TAG, "Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        ESP_LOGI(SD_TAG, "File renamed");
    } else {
        ESP_LOGI(SD_TAG, "Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    ESP_LOGI(SD_TAG, "Deleting file: %s\n", path);
    if(fs.remove(path)){
        ESP_LOGI(SD_TAG, "File deleted");
    } else {
        ESP_LOGI(SD_TAG, "Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        ESP_LOGI(SD_TAG, "%u bytes read for %lu ms\n", flen, end);
        file.close();
    } else {
        ESP_LOGI(SD_TAG, "Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        ESP_LOGI(SD_TAG, "Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    ESP_LOGI(SD_TAG, "%u bytes written for %lu ms\n", 2048 * 512, end);
    file.close();
}

void sdLog(const char * msg){
    appendFile(SD, "/output.txt", msg);
}