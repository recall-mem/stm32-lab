#ifndef HC05_INTERRUPT_H
#define HC05_INTERRUPT_H

#include "stm32f10x.h"
#include "stdint.h"


#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 64

extern char* hc05_rx_buffer;
extern char  hc05_tx_buffer[TX_BUFFER_SIZE];

extern uint8_t hc05_tx_buffer_index;
extern uint8_t hc05_tx_buffer_count;
extern uint8_t hc05_rx_buffer_index;
extern uint8_t hc05_rx_buffer_size;

extern const char* AT_CMD;
extern const char* AT_VERSION;
extern const char* AT_NAME;
extern const char* AT_UART;
extern const char* AT_ROLE;
extern const char* AT_ADDR;
extern const char* AT_PSWD;
extern const char* AT_RESTORE; // reset
extern const char* AT_RESET;

extern const char* AT_SET_PSWD_;
extern const char* AT_SET_NAME_;


void hc05_interrupt_init(uint8_t mode);
void hc05_at_mode();
void hc05_data_mode();
uint8_t hc05_connection_status();

void hc05_rx_handler();
void hc05_tx_handler();
void hc05_tc_handler();

void hc05_set_rx_buffer(char* buffer, uint8_t size);
void hc05_transmit_bytes(const char* data, uint8_t size);

#endif 