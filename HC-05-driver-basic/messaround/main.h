#include "stm32f10x.h"                  // Device header
#include "I2C_drive.h"
#include "ssd1306_drive.h"
#include "string.h"



void systick_init(void)
{
	SysTick->CTRL |= 0b100;
	SysTick->LOAD = 72000-1; //	1 kHz resolution | 1ms precision
	SysTick->VAL = 0;
}



void SDelayMs(uint32_t ms)
{
	SysTick->CTRL |= (1 << 0);
	for(uint32_t cms=0;cms<ms;cms++)
		while(!(SysTick->CTRL & (1 << 16)));
	SysTick->CTRL &= ~(1 << 0);
}



char* parse_line(char* str, uint8_t size, char* line)
{
	if(str == NULL || str[0] == '\0' || line == NULL)
			return NULL;

	char* crlf_pos = strstr(str, "\r\n");
	size_t len = crlf_pos - str;

	memcpy(line, str, len);
	line[len] = '\0';

	//	Skip cr-lf
	for(uint8_t index=0; index<(size-2); index++)
	{
		if((size_t)(index + len + 2) < 32U)	{
			str[index] = str[index + len + 2];
		}
		else	{
			str[index] = str[index + 2];
		}
	}

	return line;
}



