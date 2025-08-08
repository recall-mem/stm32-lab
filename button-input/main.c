#include "stm32f10x.h"
#include "stdint.h"

int counter = 0;
uint8_t full_cycle = 0;

void EXTI3_IRQHandler(void)
{
	EXTI->PR = (1 << 3);
	SysTick->CTRL ^= 0b001;
	counter++;
}

void SysTick_Handler(void)
{
	if(full_cycle == 250)
	{
		GPIOA->ODR ^= (1 << 2);
		full_cycle = 0;
	}
	else
	{
		full_cycle++;
	}
}

int main()
{
	RCC->APB2ENR |= 0b1101;
	
	SysTick->LOAD = 72000-1;
	SysTick->VAL = 0;
	SysTick->CTRL |= 0b110; // enable | tickint | clksource
													//									| 0-ext 1-int
	
	GPIOA->CRL &= ~(0xF << 4*2);
	GPIOA->CRL |= (1 << 4*2);
	
	GPIOB->CRL &= ~(0xF << 4*3);
	GPIOB->CRL |= (0b1000 << 4*3);
	GPIOB->ODR |= (1 << 3);
	
	AFIO->EXTICR[0] &= ~(0xF << 4*3);
	AFIO->EXTICR[0] |= (0x1 << 4*3);
	
	EXTI->RTSR |= (1 << 3);
	//EXTI->FTSR |= (1 << 3);
	
	EXTI->IMR |= (1 << 3);
	
	uint32_t GP = NVIC_GetPriorityGrouping();
	
	NVIC_SetPriority(EXTI3_IRQn, NVIC_EncodePriority(GP,10,0));
	NVIC_EnableIRQ(EXTI3_IRQn);
	
	while(1);
}
