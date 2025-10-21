#include "stm32f10x.h"  
#include "adc_drive.h"
#include "ssd1306_drive.h"
#include "stdint.h"
#include "Systick_drive.h"

#define PA1 0
#define PORTA 1

uint16_t data;  
uint16_t checker=1000;

int main()
{
	systick_init();
	ssd1306_init(1);
	adc_init(ADC_1, PORTA, PA1);
	
	ssd1306_blank(1);
	ssd1306_print_pos(1, "ADC1: ", 0x00,0x01);
	
	char arx[4] = {0,0,0,0};
	while(1)
	{
		checker--;
		if(adc_check(ADC_1)){
			data = adc_rx(ADC_1);
			for(int8_t i=3;i>=0;i--){
				arx[i] = (data%10 + 48);
				data/=10;
			}
			ssd1306_print_pos(1, arx, 0x00,0x1F);
			DelayMs(250);
		}
	}
}
