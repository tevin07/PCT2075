# PCT2075 Library

This repository contains the **PCT2075** library, which provides functions for interacting with the PCT2075 temperature sensor using the I2C protocol. The library is designed to be used with STM32 microcontrollers.

## Prerequisites

Before using this library, make sure you have the following:

- STM32 microcontroller with I2C peripheral support
- Development environment set up for STM32 (e.g., STM32CubeIDE)
- PCT2075 temperature sensor connected to your microcontroller via I2C

## Installation

To use the PCT2075 library in your project, follow these steps:

1. Clone or download this repository.
2. Copy the `pct2075.c` and `pct2075.h` files into your project folder.
3. Include the `pct2075.h` header file in your main application file.

## Usage

To use the PCT2075 library, follow the code example below:

```c
#include "pct2075.h"
#include <stdio.h>

int main(void)
{
    uint8_t i2c_status;
    PCT2075_typedef pct2075;
    ptr_pct2075_temperature temperature_value;
  
    // Configure the PCT2075 parameters
    pct2075.i2c_port = I2C1;
    pct2075.i2c_addrs = PCT2075_I2C_DEVICE_ADDRESS;
    pct2075.i2c_HAL_Master_TX = i2c_masterTransmit; // Replace with your STM32 HAL masterTransmit function
    pct2075.i2c_HAL_Master_RX = i2c_masterReceive;  // Replace with your STM32 HAL i2c_masterReceive function
  
    // Initialize the PCT2075 sensor
    PCT2075_init(&pct2075);
  
    // Read the temperature from the PCT2075 sensor
    i2c_status = PCT2075_get_temperature(&temperature_value);
  
    // Print the temperature and I2C status
    if(i2c_status == true) {
        printf("Ambient temperature: %f°C, I2C status: %d\n\r", temperature_value, i2c_status);
    }
  
    // Rest of your code...
}
````

Make sure to replace the placeholder functions i2c_masterTransmit and i2c_masterReceive with the appropriate I2C functions from the STM32 HAL library.

Contributing
If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request. Your contributions are highly appreciated.

License
This library is released under the BEER-ware license.
