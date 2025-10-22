#ifndef USART_HEADER
#define USART_HEADER

#include "stdint.h"


typedef struct {
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR[3];
	volatile uint32_t GTPR;
} USART_Typedef;

#define USART1 ((volatile USART_Typedef*) 0x40013800)
#define USART2 ((volatile USART_Typedef*) 0x40004400)
#define USART3 ((volatile USART_Typedef*) 0x40004800)

#endif