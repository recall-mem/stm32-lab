#ifndef GPIOX_HEADER
#define GPIOX_HEADER

#include "stdint.h"


typedef struct {
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
} GPIOX_typedef;

#define GPIOA ((volatile GPIOX_typedef*) 0x40010800)
#define GPIOB ((volatile GPIOX_typedef*) 0x40010C00)
#define GPIOC ((volatile GPIOX_typedef*) 0x40011000)
#define GPIOD ((volatile GPIOX_typedef*) 0x40011400)
#define GPIOE ((volatile GPIOX_typedef*) 0x40011800)
#define GPIOF ((volatile GPIOX_typedef*) 0x40011C00)
#define GPIOG ((volatile GPIOX_typedef*) 0x40012000)


#endif

