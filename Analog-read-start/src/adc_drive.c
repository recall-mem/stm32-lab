#include "stm32f10x.h"                  // Device header
#include "Systick_drive.h"
#include "adc_drive.h"


uint8_t adc_init(uint8_t adc, uint8_t port, uint8_t pin)
{
	RCC->APB2ENR |= 1; //enable afio clock
	uint8_t channel;
	uint8_t result=0;
	
	switch(port)
	{
		case 1: 
			RCC->APB2ENR |= (1 << 2);
			GPIOA->CRL &= ~(0xF << (pin * 4));
		
			channel = pin;
			result = 1;
			break;
		case 2: 
			RCC->APB2ENR |= (1 << 3);
			GPIOB->CRL &= ~(0xF << (pin * 4));
		
			channel = 8 + pin;
			result = 1;
			break;
		case 3: 
			RCC->APB2ENR |= (1 << 4);
			GPIOC->CRL &= ~(0xF << (pin * 4));
		
			channel = 10 + pin;
			result = 1;
			break;
	}
	
	switch(adc)
	{
		case ADC_1:
			RCC->APB2ENR |= (1 << 9);
			ADC1->CR1 = 0;
			ADC1->SQR3 = channel;
			ADC1->CR2 |= 1;
			DelayMs(100);
			ADC1->CR2 |= 1;
			ADC1->CR2 |= 2;
			break;
		
		case ADC_2:
			RCC->APB2ENR |= (1 << 10);
			ADC2->CR1 = 0;
			ADC2->SQR3 = channel;
			ADC2->CR2 |= 1;
			DelayMs(100);
			ADC2->CR2 |= 1;
			ADC2->CR2 |= 2;
			break;
	}
	
	return result;
}
//

uint8_t adc_check(uint8_t adc)
{
	uint8_t check=0;
	
	switch(adc)
	{
		case ADC_1:
			if(ADC1->SR & 0b10) check++;
			break;
		
		case ADC_2:
			if(ADC2->SR & 0b10) check++;
			break;
	}
	
	return check;
}
//

uint16_t adc_rx(uint8_t adc)
{
	switch(adc)
	{
		case ADC_1:
			return ADC1->DR; 
			break;
		case ADC_2:
			return ADC2->DR; 
			break;
	}
}

