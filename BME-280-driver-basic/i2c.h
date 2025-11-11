#ifndef I2C_H
#define I2C_H


#include "stdint.h"


void i2c1_init();
void generate_start();
void generate_stop();

uint8_t send_address(uint8_t addr, uint8_t mode);
void reset_addr();

void read_bytes(uint8_t buffer[], uint8_t size);
void transfer_bytes(uint8_t bytes[], uint8_t size);
void transfer_byte(uint8_t byte);
uint8_t read_byte(uint8_t ack);


#endif