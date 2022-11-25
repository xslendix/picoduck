<h1 align="center">PicoDuck</h1>

<div align="center">
    A cheap "bad" USB using a Raspberry Pi Pico running on Lua.
</div>

<br />

## Features

 - Multiple program support: allows multiple programs to be stored.
 - Hold pin support: when the configured pinned is powered, the program will pause execution.
 - No need to recompile multiple times.

## DISCLAIMER

Since this acts as effectively a mouse and keyboard, it cannot be usually detected by systems. As such, I do not condone and I am not responsible for any illegal use of this software. Use it for pranks and other legal practical uses, such as setting up systems, BUT NEVER FOR CRIME. You have been warned.

## Requirements

NOTE: Those are only needed if you are going to build the code yourself!

 - [Arduino IDE](https://www.arduino.cc/en/software)
 - [Raspberry Pi Pico Arduino core](https://github.com/earlephilhower/arduino-pico)
 - [Adafruit SPIFlash](https://github.com/adafruit/Adafruit_SPIFlash) (get from Sketch->Include Library->Manage Libraries)

## Installation

There are 2 ways of installing the code onto your Raspberry Pi Pico:
 - Getting the latest version from the releases tab
 - Compiling everything yourself

### Getting the latest version from the releases tab

WARNING: The releases version contains the default (configuration)[config.h]! Make sure your wiring corresponds to it first!

1. Head on over to the [releases](https://github.com/xslendix/picoduck/releases) page to get the latest version. You are looking for the UF2 image file.
2. Insert yout Raspberry Pi Pico into your computer while holding the BOOTSEL button down.
3. Copy the UF2 image to the newly found USB device.
4. That's it! You can now use your Raspberry Pi Pico as a bad USB!

### Compiling everything yourself

1. Install the Arduino IDE. Both 1.8.x and 2.x should work just fine.
2. Download the source code onto your computer.
3. Open the Arduino IDE and go to File->Preferences and add the following URL to "Additional Boards Manager URLs": `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`. Hit "OK".
4. Go to Tools->Boards->Board Manager, type "pico" and select "Add".
5. Go to where you downloaded the project and open the .ino file. Go to Tools->Board and select the "Raspberry Pi Pico", Tools->USB Stack "Adafruit TinyUSB".
6. Go to the "config.h" tab. Change stuff to your needs.
7. Plug in your Raspberry Pi Pico in UF2 mode.
8. Go to Tools->Port and select "UF2 Board".
9. Press the upload button, you can also use Ctrl+U.
10. That's it! You can now use your Raspberry Pi Pico as a bad USB!

## Usage

You can switch between programs using binary. For example, to load the first program, use `0001`, to load the third one use `0011`, 0 meaning false and 1 meaning true.

Programs stored in the Pico must have the following file name format: `<Program ID>.lua`.

Use the HOLD pin to halt the current execution of the program while HIGH.

Set the MSD pin HIGH when booting to put the PicoDuck into _Mass Storage Device mode_ which allows you to upload code to it.

## Documentation

### LED status codes
If the LED blinks at a rate of 1000 ms, then formatting the FAT12 partition failed. If it blinks at a rate of 500 ms, it means that PicoDuck couldn't find the program to run.

### General
```
wait(Number milliseconds) -> Wait for a x amount of time. Aliases: sleep, delay
```

### Keyboard
```
type(String) -> Type a string. Aliases: string
typeln(String) -> Type a string and press key.enter.
write(Key) -> Write a key.
press(Key) -> Press a key.
release(Key or nil) -> Release a key, if nil, releases all keys.
gui(Key) -> Press a key while the GUI key is pressed. Aliases: win, meta
alt(Key) -> Press a key while the Alt key is pressed.
ctrl(Key) -> Press a key while the Control key is pressed.
setSpeed(Number) -> Set the speed at which keys are pressed. Keys per second.
```

There is also a `keys` table which contains a bunch of useful keycodes such as: `keys.menu`, `keys.home`, `keys.f9` etc.

### Mouse
```
mouseMove(x, y) -> Move mouse x and y amount.
mouseClick(MouseButton or nil) -> Click a mouse button, if nil, left click is used.
mousePress(MouseButton or nil) -> Press a mouse button, if nil, left click is used.
mouseRelease(MouseButton or nil) -> Release a mouse button, if nil, only left click is released.
```

### I/O
```
setLed(Boolean state) -> Set the LED state.
setLedExternal(Boolean state) -> Set the external LED state.
blinkLed(Number rate or nil) -> Blink the LED, if nil, the rate is 1000. This goes into an infinite loop.
pinMode(Integer pin, PinMode) -> Set the pin mode (INPUT, OUTPUT, INPUT_PULLUP).
digitalWrite(Integer pin, Number) -> Set the digital output value of a pin (HIGH, LOW).
digitalRead(Integer pin) -> Read a pin's digital value.
analogWrite(Integer pin, Number) -> Set the analog value for a pin.
analogRead(Integet pin) -> Read a pin's analog value.
```
