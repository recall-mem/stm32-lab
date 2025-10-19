#include "ssd1306_spi_drive.h"

void ssd1306_Select(uint8_t spi12)
{
    if(spi12==1) {
        GPIOA->BSRR = GPIO_BSRR_BR9; // PA9=0(SPI1 CS)
    }
    else if(spi12==2) {
        GPIOB->BSRR = GPIO_BSRR_BR12; // PB12=0(SPI2 CS)
    }
}
//

void ssd1306_Deselect(uint8_t spi12)
{
    if(spi12==1) {
        GPIOA->BSRR = GPIO_BSRR_BS9; // PA9=1(SPI1 CS)
    }
    else if(spi12==2) {
        GPIOB->BSRR = GPIO_BSRR_BS12; // PB12=1(SPI2 CS)
    }
}
//

void ssd1306_DataMode(uint8_t spi12)
{
    if(spi12==1) {
        GPIOA->BSRR = GPIO_BSRR_BS10; // PA10=1(SPI1 DC)
    }
    else if(spi12==2) {
        GPIOB->BSRR = GPIO_BSRR_BS13; // PB13=1(SPI2 DC)
    }
}
//

void ssd1306_CommandMode(uint8_t spi12)
{
    if(spi12==1) {
        GPIOA->BSRR = GPIO_BSRR_BR10; // PA10=0(SPI1 DC)
    }
    else if(spi12==2) {
        GPIOB->BSRR = GPIO_BSRR_BR13; // PB13=0(SPI2 DC)
    }
}
//

void ssd1306_PageAddressingMode(uint8_t spi12)
{
	ssd1306_CommandMode(spi12);
	ssd1306_Select(spi12);
	SPI_TX1(spi12, 0x20); // Set Memory Addressing Mode
	SPI_TX1(spi12, 0x02); // Horizontal addressing mode
	ssd1306_Deselect(spi12);
}
//

void ssd1306_HorizontalMode(uint8_t spi12)
{
	ssd1306_CommandMode(spi12);
	ssd1306_Select(spi12);
	SPI_TX1(spi12, 0x20); // Set Memory Addressing Mode
	SPI_TX1(spi12, 0x00); // Horizontal addressing mode
	ssd1306_Deselect(spi12);
}
//

void ssd1306_Command(uint8_t spi12, uint16_t cmd)
{
	ssd1306_CommandMode(spi12);
	ssd1306_Select(spi12);
	SPI_TX1(spi12, cmd);
	ssd1306_Deselect(spi12);
}
//

void ssd1306_Data(uint8_t spi12, uint16_t data)
{
	ssd1306_DataMode(spi12);
	ssd1306_Select(spi12);
	SPI_TX1(spi12, data);
	ssd1306_Deselect(spi12);
}
//

void ssd1306_Init(uint8_t spi12)
{
	SPI_init(spi12, 0b111);
	
	// PA8 output 50mhz push-pull for RESET
	GPIOA->CRH &=	~(GPIO_CRH_CNF8 | GPIO_CRH_MODE8);
	GPIOA->CRH |= GPIO_CRH_MODE8;
	
	// PA9 output 50mhz push-pull for CS
	GPIOA->CRH &=	~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
	GPIOA->CRH |= GPIO_CRH_MODE9;
	
	// P10 output 50mhz push-pull for DS
	GPIOA->CRH &=	~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
	GPIOA->CRH |= GPIO_CRH_MODE10;
	
	ssd1306_CommandMode(spi12);
	ssd1306_Select(spi12);
	
	GPIOA->BSRR |= (1 << (8+16)); 
	TDelayMs(10);
	GPIOA->BSRR |= (1 << 8); 
	TDelayMs(100);
	
	SPI_TX1(spi12, 0xAE); // Display OFF
	
	SPI_TX1(spi12, 0xA8); // Set MUX Ratio
	SPI_TX1(spi12, 0x1F); // 1Fh for 128x32 | 3Fh for 128x64 
	
	SPI_TX1(spi12, 0xD3); 
	SPI_TX1(spi12, 0x00);
	SPI_TX1(spi12, 0x40);
	
	SPI_TX1(spi12, 0xA1); // Segment re-map (A0h <-) (A1h ->)
	SPI_TX1(spi12, 0xC8); // COM scan direction (top C8h) (bottom C0h)
	
	SPI_TX1(spi12, 0xDA); // COM Pins hardware configuration
	SPI_TX1(spi12, 0x02); // 02h for 128x32 | 12h for 128x64 
	
	SPI_TX1(spi12, 0x81); // Set Contrast Control
	SPI_TX1(spi12, 0x7F); // Range 0x00 to 0xFF
	
	//A4h Output follows RAM content (normal)
	//A5h Output ignores RAM (all pixels on)
	SPI_TX1(spi12, 0xA4); 
	
	SPI_TX1(spi12, 0xA6); // A6h pixels off A7h pixels on
	
	SPI_TX1(spi12, 0xD5); // Oscillator Frequency
	SPI_TX1(spi12, 0x80); // 80h default
	
	SPI_TX1(spi12, 0x8D); // Charge Pump Regulator
	SPI_TX1(spi12, 0x14); // 10h disable / 14h enable
	
	SPI_TX1(spi12, 0xAF); // Display ON

	ssd1306_CommandMode(spi12);
	SPI_TX1(spi12, 0x20); // Set Memory Addressing Mode
	SPI_TX1(spi12, 0x00); // Horizontal addressing mode

	//clear display
	ssd1306_DataMode(spi12);
	for(uint16_t s=0;s<512;s++) {
		ssd1306_Select(spi12);
		SPI_TX1(spi12, 0x00);
		ssd1306_Deselect(spi12);
	}
	
	ssd1306_SetPosXY(spi12, 0, 0);
	
	ssd1306_Deselect(spi12);
}
//

void ssd1306_SetPosXY(uint8_t spi12, uint32_t PAGE, uint32_t SEG)
{
	ssd1306_CommandMode(spi12);
	ssd1306_Select(spi12);
	
	SPI_TX1(spi12, (0xB0+PAGE)); 
	SPI_TX1(spi12, (0x00 + (SEG & 0x0F))); // lower nibble
	SPI_TX1(spi12, (0x10 + (SEG >> 4))); // upper nibble
	
	ssd1306_Deselect(spi12);
}
//

void ssd1306_printmsg(uint8_t spi12, const char *msg,uint8_t size)
{
	ssd1306_DataMode(spi12);
	ssd1306_Select(spi12);
	
	for(uint8_t i=0;i<size;i++) {	
		ssd1306_Select(spi12);
		for(uint8_t j=0;j<5;j++)
			SPI_TX1(spi12, ASCII_5[msg[i]-32][j]);
		SPI_TX1(spi12, 0x00); //space between chars
		ssd1306_Deselect(spi12);
	}
	
	ssd1306_Deselect(spi12);
}
//

void ssd1306_printchar(uint8_t spi12, char ch)
{
	ssd1306_DataMode(spi12);
	ssd1306_Select(spi12);
	
	for(uint8_t j=0;j<5;j++)
		SPI_TX1(spi12, ASCII_5[ch-32][j]);
	SPI_TX1(spi12, 0x00);
	
	ssd1306_Deselect(spi12);
}

void ssd1306_drawscreen(uint8_t spi12, const char *img)
{
	ssd1306_SetPosXY(spi12, 0x00, 0x00);
	
	ssd1306_CommandMode(spi12);
	ssd1306_Select(spi12);
	
	SPI_TX1(spi12, 0x20); // Set Memory Addressing Mode
	SPI_TX1(spi12, 0x00); // Horizontal addressing mode
	
	ssd1306_DataMode(spi12);

		for(uint16_t i=0;i<512;i++) {	
		ssd1306_Select(spi12);
		SPI_TX1(spi12, img[i]);
		ssd1306_Deselect(spi12);
	}
	
	ssd1306_Deselect(spi12);
}
//

void ssd1306_clear(uint8_t spi12)
{
	ssd1306_DataMode(spi12);
	ssd1306_Select(spi12);
	for(uint16_t i=0;i<512;i++)	{
		
	}
}
//


