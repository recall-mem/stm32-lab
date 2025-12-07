#include "main.h"
#include "hc05_interrupt.h"


void USART2_IRQHandler()	
{
	uint32_t sr = USART2->SR;
	
	if(sr & (1 << 5) && (USART2->CR1 & (1 << 5))) 		    hc05_rx_handler();
	else if((sr & (1 << 6)) && (USART2->CR1 & (1 << 6)))  hc05_tc_handler();
	else if(sr & (1 << 7) && (USART2->CR1 & (1 << 7)))    hc05_tx_handler();
}


char buffer[32];
char line[16];



int main()
{
	hc05_interrupt_init(0);
	systick_init();
	
	hc05_set_rx_buffer(buffer, 32);
	
	
	const char* message = "Hello Pc\r\n";
	
	hc05_transmit_bytes(message, strlen(message));
	NVIC_EnableIRQ(USART2_IRQn);
	
	while(1) __NOP();
	
	
	return 0;
}


