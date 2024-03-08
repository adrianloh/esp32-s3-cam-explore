#include <Arduino.h>

#define BOARD_GREEN_LED 34 // Active LOW
#define BUTTON_PIN 38      // Define GPIO38 as the button pin

volatile bool buttonState = false; // Used to store the current state of the button

// This is the interrupt service routine (ISR), called when the button is pressed
void IRAM_ATTR handleInterrupt()
{
  buttonState = !buttonState; // Toggle the button state
}

void setup()
{
  pinMode(BOARD_GREEN_LED, OUTPUT);  // Set the green LED pin as an output
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set the button pin as an input with an internal pull-up resistor
  // Attach the interrupt to the button pin, trigger on falling edge
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, RISING);
}

void loop()
{
  if (buttonState)
  {
    digitalWrite(BOARD_GREEN_LED, HIGH);
  }
  else
  {
    digitalWrite(BOARD_GREEN_LED, LOW);
  }
}