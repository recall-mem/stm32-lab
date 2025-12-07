#ifndef HC05_H
#define HC05_H

#include "stm32f10x.h"
#include "stdint.h"


void hc05_init();
void hc05_at_mode();
void hc05_data_mode();
uint8_t hc05_connection_state();

void hc05_transmit_byte(uint8_t byte);
void hc05_transmit_string(const char* str, uint8_t size);
	
char hc05_receive_byte();
void hc05_receive_string(char* BUFFER, uint8_t size);

//	This function used to receive responses resonses directly from the hc05 itself.
//	Received resonses such as that ending with '\r\n' after sending AT commands 
void hc05_receive_response(char* BUFFER, uint8_t size);


#endif 