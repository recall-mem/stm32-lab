#include "stm32f10x.h"                  // Device header
#include "Systick_drive.h"
#include "I2C_drive.h"
#include "ssd1306_drive.h"


int main(void)
{
	systick_init();
	DelayMs(200);
	ssd1306_init(2);
	
	DelayMs(2000);
	ssd1306_blank(2);
	DelayMs(1000);
	
	const char *msg = "Hello world";
	ssd1306_print_pos(2, msg, 0x03, 0x30);
	
	while(1);
}
