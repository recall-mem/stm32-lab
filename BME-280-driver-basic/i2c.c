#include "stm32f10x.h"                  // Device header
#include "i2c.h"



void i2c1_init()
{
	RCC->APB2ENR |= 1; 					// AFIO clock enable
	RCC->APB2ENR |= (1 << 3); 	// IOPB clock enable
	RCC->APB1ENR |= (1 << 21); 	// I2C1 clock enable
	
	
	GPIOB->CRL |= (0xF << 4*6); //PB6 opt50 open-drain
	GPIOB->CRL |= (0xF << 4*7); //PB7 opt50 open-drain
		

	I2C1->CR1 |= (1 << 15); // SWRST: Software reset
	I2C1->CR1 &= ~(1 << 15); 
		
	
	I2C1->CR2 = 36; //APB1 clock speed
		
	
	// CCR = Fpclk1 / (2 * I2C_SCL_Frequency)
	I2C1->CCR = 180;
		
	
	// TRISE = (Fpclk1 in MHz) + 1
	I2C1->TRISE = (36 + 1);
		
		
	// Enable i2c
	I2C1->CR1 |= 1;
}



void generate_start()
{
	// Wait if bus is busy
	int32_t t = 100000;
	while((I2C1->SR2 & (1 << 1)) && t--); // BUSY flag
	
	I2C1->CR1 |= (1 << 8); 	 // generate start condition
	while(!(I2C1->SR1 & 1)); // wait until start generated
}



void generate_stop() 
{
	I2C1->CR1 |= (1 << 9);  				// Generate stop condition
	while(I2C1->CR1 & (1 << 9));	  // Wait until stop generated
}



uint8_t send_address(uint8_t addr, uint8_t mode)
{
	I2C1->DR = (uint8_t)((addr << 1) | (mode & 0x01));
	
	uint16_t t=0; // timeout
	while(!(I2C1->SR1 & 0b10))	{ // wait ADDR bit
		if(t > UINT16_MAX-1)	{
			return 1;
		}	t++;
	}
	
	reset_addr();
	return 0;
}



void reset_addr()	
{
	uint32_t tmp;
	
	tmp = I2C1->SR1;
	tmp = I2C1->SR2;
	(void)tmp;
}



void transfer_bytes(uint8_t bytes[], uint8_t size)
{
	for(uint8_t c=0; c<size; c++)
	{
		while(!(I2C1->SR1 & (1 << 7))); // wait until TxE set
		
		I2C1->DR = bytes[c];
	}
	
	while(!(I2C1->SR1 & (1 << 2))); // wait until transfer complete
}



void transfer_byte(uint8_t byte)
{
	while(!(I2C1->SR1 & (1 << 7))); // wait until TxE set
	I2C1->DR = byte;
	
	while(!(I2C1->SR1 & (1 << 2))); // wait until transfer complete
}



void read_bytes(uint8_t buffer[], uint8_t size)
{
	I2C1->CR1 |= (1 << 10); // ACK set
	
	for(uint8_t c=0; c<size; c++)	
	{	
		// last byte, reset ACK bit
		if((c+1) == size)
			I2C1->CR1 &= ~(1 << 10);
		
		while(!(I2C1->SR1 & (1 << 6))); // wait for RxNE bit
		buffer[c] = I2C1->DR;
	}
}



uint8_t read_byte(uint8_t ack)
{
	if(ack)	
		I2C1->CR1 |= (1 << 10); // ACK
	else
		I2C1->CR1 &= ~(1 << 10);	// NACK
	
	
	while(!(I2C1->SR1 & (1 << 6))); // wait for RxNE bit
	return I2C1->DR;
}



