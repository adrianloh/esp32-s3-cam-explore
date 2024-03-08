#include "Arduino.h"

#define SDMMC_CLK 42
#define SDMMC_CMD 39
#define SDMMC_D0 41

#include "FS.h"
#include "SD_MMC.h"
#include "SPI.h"

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void setup() {
    
    Serial.begin(115200);

    SD_MMC.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_D0);

    // Initialize SD_MMC with the 1-line mode
    if(!SD_MMC.begin("/sdcard", true)) {
        Serial.println("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD_MMC.cardType();
    Serial.print("SD Card Type: ");
    switch (cardType) {
      case CARD_NONE:
        Serial.println("No SD card attached");
        return;
      case CARD_MMC:
        Serial.println("MMC");
        break;
      case CARD_SD:
        Serial.println("SDSC");
        break;
      case CARD_SDHC:
        Serial.println("SDHC");
        break;
      default:
        Serial.println("UNKNOWN");
        break;
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    listDir(SD_MMC, "/", 1);

}

void loop() {
    // Nothing to do here
}

