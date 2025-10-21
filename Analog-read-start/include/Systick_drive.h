#ifndef SYSTICK_HEADER
#define SYSTICK_HEADER

#include "stdint.h"
#include "stm32f10x.h"                  // Device header


void systick_init(void);	
void DelayMs(uint32_t ms);


#endif

