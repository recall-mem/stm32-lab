#ifndef STM32F1xx_DRIVE
#define STM32F1xx_DRIVE

#include "rcc.h"
#include "gpio.h"
#include "afio.h"
#include "usart.h"

typedef enum {
	PORTA=0,
	PORTB,
	PORTC,
	PORTD,
	PORTE,
	PORTF,
	PORTG
} GPIO_PORT_ENUM;

void TOGGLE_PIN(GPIO_PORT_ENUM PORT, short pin);

void INIT_PIN(GPIO_PORT_ENUM PORT, uint8_t pin, uint8_t cnfmode);

#endif