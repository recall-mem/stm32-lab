#include "timer.h"


void TIM1_init(void)
{
	RCC->APB2ENR |= (1 << 11); //TIM1 clock enable
	
	TIM1->CR1 &= ~(1 << 0); // timer is off
	TIM1->CNT = 0;
	TIM1->PSC = 72-1; // 1MHZ
	TIM1->ARR = 1000-1;	
	TIM1->CR1 |= (1 << 3); // one pulse mode
}

void TDelayUs(uint16_t Us)
{
	TIM1->ARR = Us;
	TIM1->CR1 |= 1;
	while(!(TIM1->SR & (1 << 0)));
	TIM1->SR &= ~(1 << 0);
}

void TDelayMs(uint16_t Ms)
{
	while(Ms)
	{
		Ms--;
		TDelayUs(1000);
	}
}

void TStartMeasureUs(void)
{
	TIM1->ARR = 1000;
	TIM1->CNT = 0;
	TIM1->EGR = (1 << 0);
	TIM1->CR1 |= 1;
}

uint16_t TStopMeasureUs(void)
{
	TIM1->CR1 &= ~(1 << 0);
	return TIM1->CNT;
}

