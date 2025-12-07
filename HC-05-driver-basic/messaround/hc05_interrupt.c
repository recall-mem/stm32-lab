#include "hc05_interrupt.h"


#define APB1_CLOCK_SPEED 36000000
#define BRATE_96 (APB1_CLOCK_SPEED / 9600)                      

#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 64


const char* AT_CMD        = "AT\r\n";
const char* AT_VERSION    = "AT+VERSION?\r\n";
const char* AT_NAME       = "AT+NAME?\r\n";
const char* AT_UART       = "AT+UART?\r\n";
const char* AT_ROLE       = "AT+ROLE?\r\n";
const char* AT_ADDR       = "AT+ADDR?\r\n";
const char* AT_PSWD       = "AT+PSWD?\r\n";
const char* AT_RESTORE    = "AT+ORGL\r\n";       // reset
const char* AT_RESET      = "AT+RESET\r\n";

const char* AT_SET_NAME_ = "AT+NAME=AbbasBT\r\n";
const char* AT_SET_PSWD_ = "AT+PSWD=\"1111\"\r\n";


char* hc05_rx_buffer;
char hc05_tx_buffer[TX_BUFFER_SIZE];

uint8_t hc05_tx_buffer_index = 0;
uint8_t hc05_tx_buffer_count = 0;
uint8_t hc05_rx_buffer_index = 0;
uint8_t hc05_rx_buffer_size  = 0;



void hc05_interrupt_init(uint8_t mode)
{
	//
	//	Pin configuration
	//
	
	//	enable AFIO & GPIOA + C clock
	RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 4); 

	//	PA2 AF Output50 MHz Push-Pull
	GPIOA->CRL &= ~(0xF << (4 * 2)); 
	GPIOA->CRL |=  (0b1011 << (4 * 2));
	
	//	PA3 Input Floating 
	GPIOA->CRL &= ~(0xF << (4 * 3));
	GPIOA->CRL |= (0b0100 << (4 * 3));
	
	//	PA4 output50 for switch between at/data modes
	//GPIOA->CRL &= ~(0xF	<<	(4 * 4));
	//GPIOA->CRL |= (0b0011 << (4 * 4));
	
	//	PA5 input for read state of module
	GPIOA->CRL &= ~(0xF	<<	(4 * 5));
	GPIOA->CRL |= (0b0100 << (4 * 5));
	
	//if(mode == 0)	//	data mode
		//GPIOA->BSRR |= (1 << (4 + 16));
	
	//else if(mode == 1)	//	at mode
		//GPIOA->BSRR |= (1 << 4);
	
	//for(uint32_t t=0; t<2000000; t++);
	
	//GPIOA->CRL &= ~(0xF	<<	(4 * 4));
	
	//
	//	Usart2 configuration
	//
	
	//	Enable USART2 clock
	RCC->APB1ENR |= (1 << 17); 
	
	USART2->BRR = 0xEA6;
	
	//	TE | RE | UE
	USART2->CR1 |= (0x3 << 2) | (1 << 5) | (1 << 13);
}



void hc05_rx_handler()
{
	hc05_rx_buffer[hc05_rx_buffer_index++] = (char)USART2->DR;
}



void hc05_tx_handler()
{
	if (hc05_tx_buffer_index < hc05_tx_buffer_count)
	{
		USART2->DR = hc05_tx_buffer[hc05_tx_buffer_index++];
	}
	else
	{
		USART2->CR1 &= ~(1 << 7);  // TXEIE OFF
		USART2->CR1 |= (1 << 6);   // TCIE  ON
	}
}



void hc05_tc_handler()
{
	USART2->CR1 &= ~(1 << 6);  // TCIE OFF
	// USART2->SR  &= ~(1 << 6);  // TC Reset
	
	hc05_tx_buffer_count = 0;
	hc05_tx_buffer_index = 0;
}



void hc05_transmit_bytes(const char* data, uint8_t size)
{
	if((size+hc05_tx_buffer_count) >= TX_BUFFER_SIZE)
		return;
	
	for (uint8_t i = 0; i < size; i++)
	{
    hc05_tx_buffer[hc05_tx_buffer_count + i] = data[i];
	}

	hc05_tx_buffer_count += size;
	
	USART2->CR1 |= (1 << 7);	// TXEIE ON
}



void hc05_set_rx_buffer(char* buffer, uint8_t size)
{
	hc05_rx_buffer = buffer;
	hc05_rx_buffer_size = size;
	hc05_rx_buffer_index = 0;
}



__attribute__((weak)) void hc05_at_mode()
{
	GPIOA->BSRR |= (1 << 4);
}
	
	
	
__attribute__((weak)) void hc05_data_mode()
{
	GPIOA->BSRR |= (1 << (4 + 16));
}



__attribute__((weak)) uint8_t hc05_connection_status()
{
	return (GPIOA->IDR & (1 << 5));
}




