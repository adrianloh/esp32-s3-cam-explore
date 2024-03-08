// https://github.com/Xylopyrographer/LiteLED/blob/main/examples/blink/blink.ino

#include <Arduino.h>
#include <LiteLED.h>
#include <vector>

#define BOARD_GREEN_LED 34 // Active LOW

#define LED_GPIO 33
#define LED_TYPE LED_STRIP_SK6812
#define LED_TYPE_IS_RGBW 0
#define MAX_BRIGHTNESS 32

rgb_t color1 = {.r = 181, .g = 26, .b = 237};
rgb_t color2 = {.r = 103, .g = 237, .b = 26};
rgb_t color3 = {.r = 140, .g = 158, .b = 3};

std::vector<rgb_t> colors = {color1, color2, color3};

u8_t colorIndex = 0;

LiteLED myLED(LED_TYPE, LED_TYPE_IS_RGBW);

rgb_t randomColor()
{
    rgb_t randColor = {.r = 0, .g = 0, .b = 0};
    randColor.r = random(255);
    randColor.g = random(255);
    randColor.b = random(255);
    return randColor;
}

void cycleRGB()
{
    while (1)
    {
        rgb_t currentColor = colors[colorIndex % colors.size()];
        myLED.setPixel(0, currentColor, 0); // set the LED colour but don't show it yet
        for (int i = 0; i < MAX_BRIGHTNESS; i++)
        {
            myLED.brightness(i, 1);
            delay(25);
        }
        for (int i = MAX_BRIGHTNESS; i > 0; i--)
        {
            myLED.brightness(i, 1);
            delay(25);
        }
        colorIndex++;
    }
}

void breatheRGB()
{
    while (1)
    {
        rgb_t currentColor = randomColor();
        myLED.setPixel(0, currentColor, 0); // set the LED colour but don't show it yet
        for (int i = 0; i < MAX_BRIGHTNESS; i++)
        {
            myLED.brightness(i, 1);
            delay(25);
        }
        for (int i = MAX_BRIGHTNESS; i > 0; i--)
        {
            myLED.brightness(i, 1);
            delay(25);
        }
    }
}

void blinkGreenLED(void *parameter)
{
    while (1)
    {
        digitalWrite(BOARD_GREEN_LED, HIGH);
        delay(250);
        digitalWrite(BOARD_GREEN_LED, LOW);
        delay(250);
    }
}

void setup()
{
    // Blink the green LED with the second CPU
    pinMode(BOARD_GREEN_LED, OUTPUT);
    xTaskCreatePinnedToCore(
        blinkGreenLED, // Task function
        "blinkTask",   // Task name
        10000,         // Stack size (bytes)
        NULL,          // Task parameter
        1,             // Task priority
        NULL,          // Task handle
        1              // Core number (1 for the second core)
    );

    myLED.begin(LED_GPIO, 1); // initialze the myLED object. Here we have 1 LED attached to the LED_GPIO pin
    breatheRGB();
}

void loop()
{
    // put your main code here, to run repeatedly:
}