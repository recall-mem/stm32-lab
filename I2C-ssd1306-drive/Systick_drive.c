#include "Systick_drive.h"


void systick_init(void)
{
	SysTick->CTRL |= 0b101; //clksrc | int | enb
	SysTick->LOAD = 72000-1; //1ms precision
	SysTick->VAL = 0;
}	

void DelayMs(uint32_t ms)
{
	while(ms--){
		while(!(SysTick->CTRL & (1<<16)));
	}
}

