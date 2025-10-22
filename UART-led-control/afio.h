#ifndef AFIO_HEADER
#define AFIO_HEADER

#include "stdint.h"

typedef struct {
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR[4];
	volatile uint32_t	MAPR2;
} AFIO_Typedef;

#define AFIO ((volatile AFIO_Typedef*) 0x40010000)

#endif