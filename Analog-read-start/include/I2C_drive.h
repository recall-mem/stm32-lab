#ifndef I2C_HEADER
#define I2C_HEADER

#include "stdint.h"

#define I2C_FM 0x2d
#define I2C_SM 0xB4


void i2c_init(uint8_t iic, uint8_t speed_mode);
void i2c_start(uint8_t iic);
uint8_t i2c_address(uint8_t iic, uint8_t address, uint8_t RW);
void i2c_write(uint8_t iic, uint8_t address, uint8_t data[]); //last element must be 0x00
void i2c_data(uint8_t iic, uint8_t data);
void i2c_stop(uint8_t iic);

#endif