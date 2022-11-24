#ifndef CONFIG_H
#define CONFIG_H

// LED Pins.
const uint8_t EXTERNAL_LED_PIN = 10;
const uint8_t BUILT_IN_LED_PIN = 25;

// Hold pin. This pin, if HIGH, hold the currently running program.
const uint8_t HOLD_PIN = 1;

// Mass Storage Device toggle pin. If high, the device will allow you to access the Picoduck's filesystem.
const uint8_t MSD_PIN = 0;

// If you wish to not use a pin for enabling Mass Storage, but use program index 15 (0b1111) instead, remove the comment below.
//#define DONT_USE_MSD_PIN

// These pins are responsible for program selection (and enabling Mass Storage).
const int PIN_B0 = 2; // 2^0
const int PIN_B1 = 3; // 2^1
const int PIN_B2 = 4; // 2^2
const int PIN_B3 = 5; // 2^3

// Uncomment this if you wish to remove the initial delay (WILL break typing/mouse movements on more secure systems).
//#define NO_INITIAL_DELAY

// Initial typing/mouse movement delay.
#define TYPING_DELAY 500

#endif // CONFIG_H
