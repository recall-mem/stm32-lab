#include "stm32fxxdrive.h"
#include "stdint.h"

#define BUTTON_RELEASED ((GPIOB->IDR & (1 << 9)) != 0)

int main()
{
	RCC->APB2ENR |= 0b1101; // enable afio & GPIOA - B clock
	RCC->APB1ENR |= (1 << 17); //enable usart2 clock
	
	// setting up the PINs
	// PA2 outpu50 - af push/pull
	GPIOA->CRL &= ~(0xF << 4*2); 
	GPIOA->CRL |= (0b1011 << 4*2); 
	
	// PA3 input-pu/pd
	GPIOA->CRL &= ~(0xF << 4*3);
	GPIOA->CRL |= (0b1000 << 4*3); 
	
	// PB9 input-pu for button
	GPIOB->CRH &= ~(0xF << 4*1);
	GPIOB->CRH |= (0b1000 << 4*1); 
	GPIOB->ODR |= (1 << 9);
	
	
	// usart setup
	// setup the baude rate
	USART2->BRR = 0xEA6; //0xEA6 = 9600 bps
	// usart & te enable
	USART2->CR[0] |= (1 << 13) | (1 << 3);
	
	
	const uint8_t data[4] = {0x65U, 0xFFU, 0x11U, 0x0U}; 
	uint8_t i = 0;
	
	while(1)
	{
		while(BUTTON_RELEASED);
		while(i<4)
		{
			while(!(USART2->SR & (1 << 6)));
			USART2->DR = data[i++];
		} i=0;
		while(!BUTTON_RELEASED);
	}
	
	return 0;
}


