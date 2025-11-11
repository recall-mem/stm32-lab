#include "stm32f10x.h"                  // Device header
#include "i2c.h"
#include "bme280.h"


#define bme280_REG_CTRL_MEAS   0xF4 
#define bme280_REG_CTRL_HUM		 0xF2


void blink_led(uint8_t blinks);
void led_pin();


	
bme280_reads_raw raw_data;
bme280_reads_compensated_float data;



int main()
{
	led_pin();
	i2c1_init();
	
	
	if(bme280_get_device_id() == 0x60)
		blink_led(2);
	
	for(uint16_t t=0; t < UINT16_MAX; t++); //little delay
	
	bme280_get_calibration_values();
	
	//ctrl_hum register 0xF2
	//osrs_h[2:0] = 001 -> humidity oversampling ×1	(up to ×16)
	uint8_t hum_setting = 0b101;
	bme280_write_register(bme280_REG_CTRL_HUM, hum_setting);
	
	//ctrl_meas register 0xF4
	//osrs_t[7:5] = 001 -> oversampling ×1	(up to ×16)
	//osrs_p[4:2] = 001 -> oversampling ×1	(up to ×16)
	//mode[1:0]		= 011 -> normal (sleep = 00, forced = 10 or 01, normal = 11)
	
	uint8_t osrs_t = 0b101; //×16
	uint8_t osrs_p = 0b101; //×16
	uint8_t mode	 = 0b10;	//forced (one time read, then sleep)
	uint8_t meas_setting = (uint8_t)( (osrs_t << 5) | (osrs_p << 2) | mode );
	
	bme280_write_register(bme280_REG_CTRL_MEAS, meas_setting);
	
	raw_data = bme280_get_data_raw();
	data = bme280_get_compensated_float(raw_data);
	
	blink_led(3);
	
	
	//breakpoint
	return 0;
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

void led_pin()
{
	//PC13 as output for integrated pin 
	RCC->APB2ENR |= (1 << 4);
	GPIOC->CRH &= ~(0xF << 4*5);
	GPIOC->CRH |= (0b0011 << 4*5);
	GPIOC->BSRR = (1 << 13); //led off
}
