#ifndef UART_H_
#define UART_H_

#include "stm32f10x.h"                  // Device header
#include "stdint.h"


void uart_init();

void uart_send_str(const char* str);
void uart_receive_str(char* buffer, uint8_t size);


#endif