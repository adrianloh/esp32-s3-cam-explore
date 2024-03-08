// Playback of MP3 files from SD card using I2S output and PCM5102 DAC

#include "Arduino.h"

#define LED_GPIO_NUM    34

#define SDMMC_CLK   42
#define SDMMC_CMD   39
#define SDMMC_D0    41

#define I2S_DOUT    46
#define I2S_BCLK    45
#define I2S_LRC     1
// #define I2S_MCLK    0

#include "FS.h"
#include "SD_MMC.h"
#include "SPI.h"

#include "Audio.h"
#include <vector>

Audio audio;
std::vector<char*>  playlist;

volatile int currentTrack = 0;

void errorBlink() {
    while(1) {
        digitalWrite(LED_GPIO_NUM, HIGH);
        delay(200);
        digitalWrite(LED_GPIO_NUM, LOW);
        delay(200);
    }
}

void addAudioFiles() {
    File root = SD_MMC.open("/");
    if (!root) {
        Serial.println("Failed to open SD card root");
        errorBlink();
    }
    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String fileName = file.name();
            if (fileName.endsWith(".mp3")) {
                Serial.println("Adding: " + fileName);
                 playlist.insert(playlist.begin(), strdup(file.path()));
            }
        }
        file = root.openNextFile();
    }
    root.close();
}

void setup() {
    
    Serial.begin(115200);

    SD_MMC.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_D0);

    // Initialize SD_MMC with the 1-line mode
    if(!SD_MMC.begin("/sdcard", true)) {
        Serial.println("Card Mount Failed");
        return;
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21);

    addAudioFiles();
    if (playlist.size() == 0) {
        Serial.println("No audio files found");
        errorBlink();
    }

    audio.connecttoFS(SD_MMC, playlist[currentTrack % playlist.size()]);

}

void loop() {
    audio.loop();
}

void audio_eof_mp3(const char *info) {  //end of file
    Serial.print("eof_mp3     ");
    Serial.println(info);
    currentTrack++;
    audio.connecttoFS(SD_MMC, playlist[currentTrack % playlist.size()]);
}