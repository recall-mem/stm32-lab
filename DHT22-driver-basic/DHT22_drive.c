#include "DHT22_drive.h"
#include "timer.h"
#include "ssd1306_drive.h"

uint8_t DHT22_Init(void)
{
	//4.7k pull up resistor connectet to PB0 -> 3.3v
	uint8_t t=0;
	RCC->APB2ENR |= (1 << 3); //GPIOB clock enable
	
	//PB0 output 50mhz open-drain
	GPIOB->CRL &= ~(0xF << 4*0); 
	GPIOB->CRL |= (0b0111 << 4*0);
	
	//3ms to ensure AM2302 could detect MCU's signal,
	GPIOB->BSRR |= (1 << (0 + 16)); //PB0 low
	TDelayMs(3);
	
	//then pull up and wait 40us for wait AM2302's response
	GPIOB->BSRR |= (1 << 0); //release PB0
	TDelayUs(40);
	
	//PB0 input mode
	GPIOB->CRL &= ~(0xF << 4*0);
	GPIOB->CRL |= (0b1000 << 4*0);
	
	//AM2302 will pull low the bus 80us as response signal
	while(GPIOB->IDR & (1 << 0)){
		t++; if(t>20) return 2;
		TDelayUs(10);
	} t=0;

	//then AM2302 pulls up for 80us for preparation to send data
	//GPIOB->ODR &= ~(1 << 0); //pull down to remove noise
	while((GPIOB->IDR & (1 << 0)) == 0){
		t++; if(t>20) return 3;
		TDelayUs(10);
	} t=0;
	
	while(GPIOB->IDR & (1 << 0)); //wait for sensor pull low again
	GPIOB->ODR |= (1 << 0); //release PB0
	return 0; //success
}




