#include "stm32fxxdrive.h"
#include "stdint.h"
#include "string.h"

#define BUTTON_RELEASED ((GPIOB->IDR & (1 << 9)) != 0)
#define COMMAND_LENGTH 8 


char read_byte(void);
void send_status(void);
void read_command(char* BUFFER, uint8_t SIZE);
void execute_command(char* BUFFER);
void blink_led(uint8_t blinks);
void clear_buffer(char *BUFFER, uint8_t SIZE);


static char COMMAND_BUFFER[COMMAND_LENGTH+1]; // 1 byte for null-terminator
static char BYTE;
static uint8_t SR;


int main()
{
	RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 4); // enable afio & GPIOA - C clock
	RCC->APB1ENR |= (1 << 17); //enable usart2 clock
	
	// setting up the PINs
	// PC13 as output for integrated pin 
	GPIOC->CRH &= ~(0xF << 4*5);
	GPIOC->CRH |= (0b0011 << 4*5);
	GPIOC->BSRR = (1 << 13); //led off
	
	// PA2 outpu50 - af push/pull for TX
	GPIOA->CRL &= ~(0xF << 4*2); 
	GPIOA->CRL |= (0b1011 << 4*2); 
	
	// PA3 input-pu/pd for RX
	GPIOA->CRL &= ~(0xF << 4*3);
	GPIOA->CRL |= (0b1000 << 4*3); 
	
	// PB9 input-pu for button
	//GPIOB->CRH &= ~(0xF << 4*1);
	//GPIOB->CRH |= (0b1000 << 4*1); 
	//GPIOB->ODR |= (1 << 9);
	
	// USART2 
	// setup the baude rate
	USART2->BRR = 0xEA6; //0xEA6 = 9600 bps
	// ue | te | re 
	USART2->CR[0] |= (1 << 13) | (1 << 3) | (1 << 2);
	

	while(1)
	{
		SR = 0;
		BYTE = read_byte();
		clear_buffer(COMMAND_BUFFER, COMMAND_LENGTH);
		
		if(BYTE == 0x00)
			continue;
		
		else if(BYTE == 'C') {
			read_command(COMMAND_BUFFER, COMMAND_LENGTH);
			SR = 1;
		}
		
		if(SR)
			execute_command(COMMAND_BUFFER);
		
		send_status();
	}
	
	return 0;
}



void read_command(char* BUFFER, uint8_t SIZE)
{
	uint8_t idx=0;
	uint16_t t=0; //timeout
	
	BUFFER[idx++] = read_byte();
	
	_continue_read:
	
	// wait for RXNE bit
	while((USART2->SR & (1<<5)) == 0)
	{
		if(++t == 0xFFFF)	{
			strcpy(BUFFER, "TIMEOUT");
			return;
		}
	}	t=0;
	
	BUFFER[idx] = (char)(USART2->DR & 0xFF);
	idx++;
	
	if(idx == SIZE) return;
	
	// if \r\n is not received continue reading
	if(BUFFER[idx-2] != '\r' || BUFFER[idx-1] != '\n')	{	
		goto _continue_read;
	}
	
	BUFFER[idx] = '\0'; // add null-terminator for strcmp
}



void execute_command(char* BUFFER)
{
	if(!strcmp(BUFFER, "LEDON\r\n")) {
		GPIOC->BSRR = (1 << (13+16)); SR = 2U;
	}
	else if(!strcmp(BUFFER, "LEDOFF\r\n")) {
		GPIOC->BSRR = (1 << 13); SR = 3U;
	}
	else if(!strcmp(BUFFER, "BLINK\r\n")) {
		blink_led(5); SR = 4U;
	}
	else
		SR = 0x0B;
}


void send_status(void)
{
	while (!(USART2->SR & (1 << 7))); // wait TXE bit set
	USART2->DR = (0xF0 | SR);
	while(!(USART2->SR & (1 << 6))); // wait for TC bit set
}



char read_byte(void)
{
	uint16_t t=0; //timeout
	while((USART2->SR & (1<<5)) == 0)
	{
		if(++t == 0xFFFF)
			return 0x00;
	}
	return (uint8_t)(USART2->DR & 0xFF);
}



void clear_buffer(char *BUFFER, uint8_t SIZE)
{
	for(uint8_t i=0; i<SIZE; i++)
		BUFFER[i] = 0;
}


void blink_led(uint8_t blinks)
{
	for(;blinks>0;blinks--) {
		GPIOC->BSRR = (1 << (13+16));
		for(uint32_t i=0;i<1000000;i++);
		GPIOC->BSRR = (1 << 13);
		for(uint32_t i=0;i<1000000;i++);
	}
}