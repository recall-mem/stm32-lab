#pragma once

#include "stm32f10x.h"                  // Device header
#include "stdint.h"
#include "timer.h"
#include "DHT22_drive.h"
#include "ssd1306_spi_drive.h"

void show_screen(int16_t val);
void Measure_contitions(void);
void PrintTempRH(char temp[], char rh[]);
void blink_led(uint8_t blinks);
void systick_init(void);
void SDelayMs(uint32_t ms);

void PB0_pinconfig(void);
void PB0_intenable(void);
void PB0_intdisable(void);


