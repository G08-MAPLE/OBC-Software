#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "spiffs.hpp"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

Filesys::Filesys() {
  ESP_LOGI(SPIFFS_TAG, "SPIFFS object created");
}

 void Filesys::config() {
    /* This function will initialize the SPIFFS filesystem. Once the filesystem has been configured it will check that the 
    desired partition is valid for the size of the system using the _partitionCheck() function. It will then verify the size
    of the partition is consisten using the _verifyPartitionSize() function. Nesting these functions makes initializing the
    filesystem simpler since it can be configured in line of code.*/

    //program was did not like conf being a variable, threw a fit on boot
  esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = false
  };  
  
  ESP_LOGI(SPIFFS_TAG, "Initializing SPIFFS");

  // Use settings defined above to initialize and mount SPIFFS filesystem.
  // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK) {
      if (ret == ESP_FAIL) {
          ESP_LOGE(SPIFFS_TAG, "Failed to mount or format filesystem");
      } else if (ret == ESP_ERR_NOT_FOUND) {
          ESP_LOGE(SPIFFS_TAG, "Failed to find SPIFFS partition");
      } else {
          ESP_LOGE(SPIFFS_TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
      }
      return;
  }

// I don't totally understand what this is doing, but it was in the example program so I'll keep it for now
  #ifdef CONFIG_EXAMPLE_SPIFFS_CHECK_ON_START
    ESP_LOGI(SPIFFS_TAG, "Performing SPIFFS_check().");
    ret = esp_spiffs_check(_conf.partition_label);
    if (ret != ESP_OK) {
        ESP_LOGE(SPIFFS_TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
        return;
    } else {
        ESP_LOGI(SPIFFS_TAG, "SPIFFS_check() successful");
    }
  #endif
    _partitionSizeCheck(ret);
    return;
}

void Filesys::_partitionSizeCheck(esp_err_t partition) {
    /* This function will check to make sure the requested partition size is less than the available memory size.*/

    size_t total = 0, used = 0;
    partition = esp_spiffs_info(conf.partition_label, &total, &used);
    if (partition != ESP_OK) {
        ESP_LOGE(SPIFFS_TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(partition));
        esp_spiffs_format(conf.partition_label);
        return;
    } else {
        ESP_LOGI(SPIFFS_TAG, "Partition size: total: %d, used: %d", total, used);
    }
    _verifyPartitionSize(total, used);
}

void Filesys::_verifyPartitionSize(size_t sizeT, size_t usedT) {
    // Check consistency of reported partiton size info. Requires total size of partition and used size of memory.
    
    if (usedT > sizeT) {
        ESP_LOGW(SPIFFS_TAG, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
        //reuses prior variable name, but should be fine based on scope of function
        esp_err_t ret = esp_spiffs_check(conf.partition_label);
        // Could be also used to mend broken files, to clean unreferenced pages, etc.
        // More info at https://github.com/pellepl/spiffs/wiki/FAQ#powerlosses-contd-when-should-i-run-spiffs_check
        if (ret != ESP_OK) {
            ESP_LOGE(SPIFFS_TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
            return;
        } else {
            ESP_LOGI(SPIFFS_TAG, "SPIFFS_check() successful");
        }
    }
}

// Use POSIX and C standard library functions to work with files.

void Filesys::createFile() {
    // First create a file.
    ESP_LOGI(SPIFFS_TAG, "Opening file");
    FILE* f = fopen("/spiffs/hello.txt", "w");
    if (f == NULL) {
        ESP_LOGE(SPIFFS_TAG, "Failed to open file for writing");
        return;
    }
    //With file open, you can write data like this
    fprintf(f, "Hello World!\n");
    fclose(f);
    ESP_LOGI(SPIFFS_TAG, "File written");
}

void Filesys::findFile() {
    // Check if destination file exists before renaming
    struct stat st;
    if (stat("/spiffs/foo.txt", &st) == 0) {
        // Delete it if it exists
        unlink("/spiffs/foo.txt");
    }
}

void Filesys::renameFile() {
    // Rename original file
    ESP_LOGI(SPIFFS_TAG, "Renaming file");
    if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0) {
        ESP_LOGE(SPIFFS_TAG, "Rename failed");
        return;
    }
}

void Filesys::readFile() {
    // Open renamed file for reading
    ESP_LOGI(SPIFFS_TAG, "Reading file");
    FILE* f = fopen("/spiffs/foo.txt", "r");
    if (f == NULL) {
        ESP_LOGE(SPIFFS_TAG, "Failed to open file for reading");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(SPIFFS_TAG, "Read from file: '%s'", line);
}

void Filesys::closeFileSys() {
    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(SPIFFS_TAG, "SPIFFS unmounted");
}