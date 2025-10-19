#ifndef TIMER_HEADER
#define TIMER_HEADER

#include "stdint.h"
#include "stm32f10x.h"                  // Device header


void TIM1_init(void);
void TDelayUs(uint16_t Us);
void TDelayMs(uint16_t Ms);
void TStartMeasureUs(void);
uint16_t TStopMeasureUs(void);

#endif