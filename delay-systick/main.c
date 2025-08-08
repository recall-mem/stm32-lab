#include "stm32f10x.h"                  // Device header

uint32_t tick=0;

void SysTick_Handler(void)
{
	tick++;
}

void DelayMS(uint32_t MS)
{
	uint32_t start = tick;
	while((tick - start) < MS);
}

void SetSysTick(void)
{
	SysTick->CTRL = 0b111;
	SysTick->VAL = 0;
	SysTick->LOAD = 72000-1;
}


int main(void)
{

	RCC->APB2ENR |= (1U << 2);
	
	GPIOA->CRL &= ~(0xF << 0);
	GPIOA->CRL |= (0b0001 << 0);

	SetSysTick();
	
	while(1)
	{
		GPIOA->ODR ^= (1U << 0);
		DelayMS(200);
	}
	
}

