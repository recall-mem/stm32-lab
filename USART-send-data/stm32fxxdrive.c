#include "stm32fxxdrive.h"


void TOGGLE_PIN(GPIO_PORT_ENUM PORT, short pin)
{
	switch(PORT)
	{
		case 0: GPIOA->ODR ^= (1 << pin); break;
		case 1: GPIOA->ODR ^= (1 << pin); break;
		default: return;
	}
};

void INIT_PIN(GPIO_PORT_ENUM PORT, uint8_t pin, uint8_t cnfmode )
{
	volatile uint32_t* CR;;
	volatile GPIOX_typedef* portx;
	
	switch (PORT)
	{
		case PORTA: portx = GPIOA; break;
		case PORTB: portx = GPIOB; break;
		case PORTC: portx = GPIOC; break;
		case PORTD: portx = GPIOD; break;
		case PORTE: portx = GPIOE; break;
		case PORTF: portx = GPIOF; break;
		case PORTG: portx = GPIOG; break;
	}
	
	if(pin>7){ CR = &portx->CRH; pin-=8;}
	else {CR = &portx->CRL;}
	
	*CR &= ~(0xF << pin * 4);
	*CR |= (cnfmode << pin * 4);
}
