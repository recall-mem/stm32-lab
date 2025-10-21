#ifndef ADC_HEADER
#define ADC_HEADER

#include "stdint.h"


uint8_t adc_init(uint8_t adc, uint8_t port, uint8_t pin);
uint8_t adc_check(uint8_t adc);
uint16_t adc_rx(uint8_t adc);


#define ADC_1 1
#define ADC_2 2



#endif