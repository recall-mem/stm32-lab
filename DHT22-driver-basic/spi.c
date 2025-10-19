#include "spi.h"


void SPI_init(uint8_t spi12, uint8_t brate_div)
{
	switch(spi12)
	{
		case 1:
			RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;
		
			// PA4 output 50mhz push-pull for NSS
			GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4); 
			GPIOA->CRL |= GPIO_CRL_MODE4; 
	
			// PA5 output 50mhz alternative function push-pull for SCL
			GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5); 
			GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5); 
	
			// PA6 input alternative function for MISO
			GPIOA->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6); 
			GPIOA->CRL |= GPIO_CRL_CNF6_1; 
	
			// PA7 output 50mhz alternative function push-pull MOSI
			GPIOA->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7); 
			GPIOA->CRL |= (GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7); 
			
			SPI1->CR1 |= SPI_CR1_MSTR; // master mode 
			SPI1->CR1 |= (brate_div << 3);
			SPI1->CR1 |= SPI_CR1_SSM; // Software Mode
			SPI1->CR1 |= SPI_CR1_SSI; // Slave not selected
			SPI1->CR2 &= ~SPI_CR2_SSOE; // HW NSS control disable
			
			SPI1->CR1 &= ~SPI_CR1_DFF; // 8-bit data frame format
			SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // MSB transmitted first
			SPI1->CR1 |= SPI_CR1_SPE; //SPI enable
			break;
		
		case 2:
			RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
		
			// PA12 output 50mhz push-pull aka NSS
			GPIOA->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_MODE12); 
			GPIOA->CRH |= GPIO_CRH_MODE12; 
	
			// PA13 output 50mhz alternative function push-pull
			GPIOA->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13); 
			GPIOA->CRH |= (GPIO_CRH_CNF13_1 | GPIO_CRH_MODE13); 
	
			// PA14 input alternative function
			GPIOA->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14); 
			GPIOA->CRH |= GPIO_CRH_CNF14_1; 
	
			// PA15 output 50mhz alternative function push-pull
			GPIOA->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15); 
			GPIOA->CRH |= (GPIO_CRH_CNF15_1 | GPIO_CRH_MODE15); 
		
			SPI2->CR1 |= SPI_CR1_MSTR; // master mode 
			SPI2->CR1 |= (brate_div << 3);
			SPI2->CR1 |= SPI_CR1_SSM; // Software Mode
			SPI2->CR1 |= SPI_CR1_SSI; // Slave not selected
			SPI2->CR2 &= ~SPI_CR2_SSOE; // HW NSS control disable
			
			SPI2->CR1 &= ~SPI_CR1_DFF; // 8-bit data frame format
			SPI2->CR1 &= ~SPI_CR1_LSBFIRST; // MSB transmitted first
			SPI2->CR1 |= SPI_CR1_SPE; //SPI enable
			break;
	};
}

void SPI_TX1(uint8_t spi12, uint8_t data)
{
	switch(spi12)
	{
		case 1:
				SPI1->DR = data;
         			break;

		case 2:
				SPI2->DR = data;
			break;
	}
	while(SPI1->SR & SPI_SR_BSY); // Must wait till transmissions ends
}

void SPI_MSG(uint8_t spi12, uint16_t* str, uint16_t size)
{
	switch(spi12)
	{
		case 1:
				for(uint16_t i=0;i<size;i++)
				{
					SPI1->DR = str[i];
					while(!(SPI1->SR & SPI_SR_TXE)); 
				}
			break;

		case 2:
				for(uint16_t i=0;i<size;i++)
				{
					SPI2->DR = str[i];
					while(!(SPI2->SR & SPI_SR_TXE)); 
				}
			break;
	}
}
