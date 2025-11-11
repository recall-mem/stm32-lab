# BME280 Bare-Metal Basic Driver (STM32 / C)

This project provides a minimal, **bare-metal** I²C driver for the **BME280 environmental sensor**.  
It reads **temperature**, **pressure**, and **humidity** without using HAL, external libraries or frameworks.

The driver is written for **STM32F1** family microcontrollers, but can be easily ported to any ARM Cortex-M MCU.

---

## Features

- Pure register-level I²C communication
- No HAL, no RTOS, no Arduino layer
- Correct handling of BME280 calibration data
- Compensation formulas implemented according to Bosch datasheet
- Supports:
  - Forced mode measurement cycle
  - Polling via `status` register (no fixed delays)

---

## Files

bme280.c

# Demo

https://youtu.be/631S0D0caCI
bme280.h
i2c.c
i2c.h

main.c
