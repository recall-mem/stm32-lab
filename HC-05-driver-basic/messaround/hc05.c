#include "hc05.h"


#define APB1_CLOCK_SPEED 36000000
#define BRATE_96 (APB1_CLOCK_SPEED / 9600)                      



void hc05_init()
{
	//	Enable afio & GPIOA - C clock
	RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 4); 
	//	Enable usart2 clock
	RCC->APB1ENR |= (1 << 17); 


	//	PA2 outpu50 - af open-drain for TX
	GPIOA->CRL &= ~(0xF << (4 * 2)); 
	GPIOA->CRL |=  (0b1011 << (4 * 2));
	
	//	PA3 input floating for RX
	GPIOA->CRL &= ~(0xF	<<	(4 * 3));
	GPIOA->CRL |= (0b0100	<<	(4 * 3));
	
	//	PA4 output50 for switch between at/data modes
	GPIOA->CRL &= ~(0xF	<<	(4 * 4));
	GPIOA->CRL |= (0b0011 << (4 * 4));
	GPIOA->BSRR |= (1 << (4 + 16)); //default data mode
	
	//	PA5 input for read state of module
	GPIOA->CRL &= ~(0xF	<<	(4 * 5));
	GPIOA->CRL |= (0b0100 << (4 * 5));
	
	
	//	Setup the baud rate
	USART2->BRR = BRATE_96;
	//	ue | te | re 
	USART2->CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}



void hc05_at_mode()
{
	GPIOA->BSRR |= (1 << 4);
}
	
	
	
void hc05_data_mode()
{
	GPIOA->BSRR |= (1 << (4 + 16));
}



uint8_t hc05_connection_state()
{
	return (GPIOA->IDR & (1 << 5));
}



void hc05_transmit_byte(uint8_t byte)
{
	while(!(USART2->SR & (1 << 7)));
	USART2->DR = byte;
}



void hc05_transmit_string(const char* str, uint8_t size)
{
	for(uint8_t c=0; c<size; c++)	
	{
		while(!(USART2->SR & (1 << 7)));
		USART2->DR = (uint8_t)str[c];
	}
}



//	Blocking a byte receiver function
char hc05_receive_byte()
{
	while(!(USART2->SR & (1 << 5)));
	return (char)USART2->DR;
}



//	Blocking receiver function (ending must be with CR+LF)
void hc05_receive_string(char* BUFFER, uint8_t size)
{
	BUFFER[0] = hc05_receive_byte();
	
	for(uint8_t c=1; c<size; c++)	
	{
		while(!(USART2->SR & (1 << 5)));
		
		BUFFER[c] = (char)USART2->DR;
		
		if(BUFFER[c-1] == '\r' && BUFFER[c] == '\n')
		{
			BUFFER[c+1] = '\0';
			return;
		}
	}
}



//	Blocking response receiver function.
//	Because HC-05 can send responses that contains multiple CR-LF
//	End of reading only happens when timeout expires
void hc05_receive_response(char* BUFFER, uint8_t size)
{
	//	wait for the fist byte to arrive
	BUFFER[0] = hc05_receive_byte();
	uint16_t timeout = 0;
	
	for(uint8_t c=1; c<size; c++)	
	{
		while(!(USART2->SR & (1 << 5)) && timeout < UINT16_MAX) timeout++;
		
		if(timeout == UINT16_MAX) //	timeout expires
		{
			BUFFER[c+1] = '\0'; 
			return;
		}
		
		BUFFER[c] = (char)USART2->DR;
	}
}
