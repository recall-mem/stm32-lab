#include "main.h"
#include "string.h"


#define SPI_1 1

int8_t bit_num=(32-1);
uint32_t data=0;
uint16_t TEMP;
uint16_t RH;


void EXTI0_IRQHandler(void)
{
	EXTI->PR = (1 << 0);
	
	if(bit_num < 0) { //ignore checksum bits

		return;
	}
	
	if(GPIOB->IDR & (1 << 0)) { //RISING edge trigger
		TStartMeasureUs();
	}

	else { //FALLING edge trigger
		if(TStopMeasureUs() > 35) {
			data |= (1 << bit_num);
		}
		bit_num--;
	}
}



int main()
{
	RCC->APB2ENR |= (1 << 0); //AFIO clock enable
	TIM1_init();
	systick_init();
	ssd1306_Init(SPI_1);
	
	while(1)
	{
		blink_led(2);
		Measure_contitions();
		SDelayMs(2100);
	}
	
	return 0;
}



void Measure_contitions(void)
{
	data = 0;
	bit_num = 31;
	
	DHT22_Init();
	
	PB0_pinconfig();
	PB0_intenable();
	
	uint32_t timeout=0;
	while(bit_num > 0 && timeout < 10000000) timeout++;
		
	PB0_intdisable();
		
	TEMP = (uint16_t)(data & 0x0000FFFF);
	RH = (uint16_t)((data & 0xFFFF0000) >> 16);
	
	char temp[3] = {0,0,0}; 
	char rh[3] = {0,0,0};
	for(int8_t i=2;i>=0;i--) {
		temp[i] = (TEMP%10 + 48);
		rh[i] = (RH%10 + 48);
		TEMP/=10;
		RH/=10;
	}
	
	PrintTempRH(temp, rh);
}

void PrintTempRH(char temp[], char rh[])
{
	ssd1306_SetPosXY(SPI_1, 0, 1);
	const char* pref[2] = {"TEMP:", "RH:"};
	ssd1306_printmsg(SPI_1, pref[0], strlen(pref[0]));
	for(uint8_t j=0;j<2;j++) 
		ssd1306_printchar(SPI_1, temp[j]);
	ssd1306_printchar(SPI_1, '.');
	ssd1306_printchar(SPI_1, temp[2]);
	ssd1306_printchar(SPI_1, 'C');
	
	ssd1306_SetPosXY(SPI_1, 2, 1);
	
	ssd1306_printmsg(SPI_1, pref[1], strlen(pref[1]));
	for(uint8_t j=0;j<2;j++) 
		ssd1306_printchar(SPI_1, rh[j]);
	ssd1306_printchar(SPI_1, '.');
	ssd1306_printchar(SPI_1, rh[2]);
	ssd1306_printchar(SPI_1, '%');
}



void PB0_intdisable(void)
{
    NVIC_DisableIRQ(EXTI0_IRQn);
    EXTI->IMR &= ~(1 << 0);
    
    //clear any pending flags
    EXTI->PR = (1 << 0);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    
		//just in case
    //disable edge detection for complete silence
    EXTI->RTSR &= ~(1 << 0);
    EXTI->FTSR &= ~(1 << 0);
}



void PB0_intenable(void)
{
    EXTI->PR = (1 << 0);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    
    EXTI->RTSR |= (1 << 0);
    EXTI->FTSR |= (1 << 0);
    EXTI->IMR |= (1 << 0);
    
    uint32_t PG = NVIC_GetPriorityGrouping();
    NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(PG, 0, 0));
    NVIC_EnableIRQ(EXTI0_IRQn);
}



void PB0_pinconfig(void)
{
    //PB0 input pull down 
    GPIOB->CRL &= ~(0xF << 4*0);
    GPIOB->CRL |= (0b0100 << 4*0); 
    
    AFIO->EXTICR[0] &= ~(0xF << 0);
    AFIO->EXTICR[0] |= (0x1 << 0);   
}



void blink_led(uint8_t blinks)
{
	RCC->APB2ENR |= (1 << 4); //enable IOPC clock
	
	//PC13 as output for integrated pin 
	GPIOC->CRH &= ~(0xF << 4*5);
	GPIOC->CRH |= (0b0011 << 4*5);
	GPIOC->BSRR = (1 << 13); //led off
	
	for(;blinks>0;blinks--) {
		GPIOC->BSRR = (1 << (13+16));
		SDelayMs(100);
		GPIOC->BSRR = (1 << 13);
		SDelayMs(100);
	}
}


void systick_init(void)
{
	SysTick->CTRL |= 0b100;
	SysTick->LOAD = 72000-1; // 1 kHz resolution | 1ms precision
	SysTick->VAL = 0;
}



void SDelayMs(uint32_t ms)
{
	SysTick->CTRL |= (1 << 0);
	for(uint32_t cms=0;cms<ms;cms++)
		while(!(SysTick->CTRL & (1 << 16)));
	SysTick->CTRL &= ~(1 << 0);
}



