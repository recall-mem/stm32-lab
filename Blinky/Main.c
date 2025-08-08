#include "stm32f10x.h"                  // Device header


void delay(int reps);

int main(void)
{
	RCC->APB2ENR &= 0x2; // enable IOPA clock
	
	GPIOA->CRL &= ~(0xF << 4*3);
	GPIOA->CRL |= (0b0001 << 4*3);
	
	while(1)
	{
		GPIOA->ODR ^= (1 << 3);
		delay(1);
	}
	
}

void delay(int reps)
{
	while(reps--)
		{for(volatile int i=0;i<1000000;++i);}
}
