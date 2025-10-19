#ifndef SPI_HEADER
#define SPI_HEADER

#include "stm32f10x.h"                  // Device header


void SPI_init(uint8_t spi12, uint8_t brate);

void SPI_TX1(uint8_t spi12, uint8_t data);
void SPI_MSG(uint8_t spi12, uint16_t* str, uint16_t size);


#endif