#ifndef RCC_HEADER
#define RCC_HEADER

#include "stdint.h" //for uint32_t


typedef struct {
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

#define RCC ((volatile RCC_typedef*) 0x40021000)


#endif