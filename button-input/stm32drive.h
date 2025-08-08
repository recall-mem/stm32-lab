#ifndef STM32F1XX_DRIVE
#define STM32F1XX_DRIVE
#include "stdint.h"

typedef struct{
	volatile uint32_t CR;         
	volatile uint32_t CFGR;       
	volatile uint32_t CIR;        
	volatile uint32_t APB2RSTR;   
	volatile uint32_t APB1RSTR;   
	volatile uint32_t AHBENR;     
	volatile uint32_t APB2ENR;    
	volatile uint32_t APB1ENR;    
	volatile uint32_t BDCR;       
	volatile uint32_t CSR;        
} RCC_typedef;



typedef enum {
	PORTA=0,
	PORTB,
	PORTC,
	PORTD,
	PORTE,
	PORTF,
	PORTG
} GPIO_PORT_ENUM;



typedef struct {
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
} GPIOX_typedef;



#define RCC ((volatile RCC_typedef*) 0x40021000)
	
#define GPIOA ((volatile GPIOX_typedef*) 0x40010800)
#define GPIOB ((volatile GPIOX_typedef*) 0x40010C00)
#define GPIOC ((volatile GPIOX_typedef*) 0x40011000)
#define GPIOD ((volatile GPIOX_typedef*) 0x40011400)
#define GPIOE ((volatile GPIOX_typedef*) 0x40011800)
#define GPIOF ((volatile GPIOX_typedef*) 0x40011C00)
#define GPIOG ((volatile GPIOX_typedef*) 0x40012000)



void TOGGLE_PIN(GPIO_PORT_ENUM PORT, short pin);

// Must define it (maybe later)
void INIT_PIN(GPIO_PORT_ENUM PORT, uint8_t pin, uint8_t cnfmode);

#endif

