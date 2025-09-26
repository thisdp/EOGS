# EOGS (Embedded OLED Graphical System)

A lightweight OLED GUI library for embedded systems.

## Overview

EOGS is a lightweight GUI library designed for embedded systems with OLED displays. It provides a set of widgets and tools to create graphical user interfaces for microcontroller-based projects.

## Features

- Lightweight and efficient
- Supports various OLED display drivers through U8g2 library
- Provides a set of customizable widgets
- Animation support
- Input handling

## Widgets

- Label
- Rectangle
- Circle
- Menu
- SwitchButton

## SwitchButton Widget

The SwitchButton widget is a toggle switch control that can be used to represent on/off or enable/disable states.

### Usage

```cpp
#include "EOGS.h"
#include "widgets/switchbutton.h"

// Create a switch button
EOGSSwitchButton* switchBtn = eogs.create<EOGSSwitchButton>(x, y, width, height, isRelative);

// Set the initial state
switchBtn->setChecked(true);  // true for ON, false for OFF

// Get the current state
bool state = switchBtn->getChecked();
```

### Example

```cpp
#include <Arduino.h>
#include <U8g2lib.h>
#include "EOGS.h"
#include "EOGS_U8G2_HAL.h"
#include "widgets/switchbutton.h"

// U8g2 constructor (modify according to your hardware configuration)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// EOGS HAL instance
U8G2_EOGS_HAL hal(u8g2);

// EOGS instance
EOGS eogs(hal, 128, 64);

void setup() {
  // Initialize serial
  Serial.begin(115200);
  
  // Initialize U8g2
  u8g2.begin();
  
  // Create a switch button
  EOGSSwitchButton* switchBtn = eogs.create<EOGSSwitchButton>(10, 20, 20, 20, false);
  switchBtn->setChecked(false);
}

void loop() {
  // Render the EOGS interface
  eogs.render();
  
  delay(100);
}
```

### Methods

- `setChecked(bool checked)` - Set the switch state
- `getChecked()` - Get the current switch state
- `setIndicatorColorOn(DrawColor color)` - Set the indicator color when switch is on
- `getIndicatorColorOn()` - Get the indicator color when switch is on
- `setIndicatorColorOff(DrawColor color)` - Set the indicator color when switch is off
- `getIndicatorColorOff()` - Get the indicator color when switch is off
- `setBgColorOn(DrawColor color)` - Set the background color when switch is on
- `getBgColorOn()` - Get the background color when switch is on
- `setBgColorOff(DrawColor color)` - Set the background color when switch is off
- `getBgColorOff()` - Get the background color when switch is off

## Installation

1. Download or clone this repository
2. Place the EOGS folder in your Arduino libraries directory
3. Install the U8g2 library if not already installed

## Dependencies

- U8g2 library for OLED display support

## License

This project is licensed under the MIT License - see the LICENSE file for details.