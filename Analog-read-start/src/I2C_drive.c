#include "stm32f10x.h"                  // Device header
#include "I2C_drive.h"
#include "stdint.h"
#include "Systick_drive.h"

void i2c_init(uint8_t iic, uint8_t speed_mode)
{
	RCC->APB2ENR |= 1;
	RCC->APB2ENR |= (1 << 3);
	
	GPIOB->CRH &= ~(0xF << 4*1);
	GPIOB->CRH |= (0b0011 << 4*1);
	
	if(iic == 1){
		RCC->APB1ENR |= (1 << 21);
		
		GPIOB->CRL |= (0xF << 4*6); //PB6 opt50 open-drain
		GPIOB->CRL |= (0xF << 4*7); //PB7 opt50 open-drain
		
		I2C1->CR1 |= (1 << 15); //reset mode
		I2C1->CR1 &= ~(1 << 15);	//exit reset		
		
		I2C1->CR2 = 0x8;
		
		I2C1->CCR = speed_mode;
		I2C1->TRISE = 0x9;
		
		I2C1->CR1 |= 1;
	}
	else if(iic == 2){
		RCC->APB1ENR |= (1 << 22);
		
		GPIOB->CRH |= (0xF << 4*2); //PB10 opt50 open-drain
		GPIOB->CRH |= (0xF << 4*3); //PB11 opt50 open-drain
		
		I2C2->CR1 |= (1 << 15); //reset mode
		I2C2->CR1 &= ~(1 << 15);	//exit reset		
		
		I2C2->CR2 = 0x8;
		
		I2C2->CCR = speed_mode;
		I2C2->TRISE = 0x9;
		
		I2C2->CR1 |= 1; 
	}
}
//

void i2c_start(uint8_t iic)
{
	if(iic == 1){
		I2C1->CR1 |= (1 << 8);
		
		while(!(I2C1->SR1 & 1)); //wait until SR1.SB is 1
	}
	else if(iic == 2){
		I2C2->CR1 |= (1 << 8);
		
		while(!(I2C2->SR1 & 1)); //wait until SR1.SB is 1
	}
}
//

uint8_t i2c_address(uint8_t iic, uint8_t address, uint8_t RW)
{
	uint32_t tmp;
	
	if(iic == 1){
		I2C1->DR = ((address << 1) | RW);  
		
		uint32_t tm = 30000000; //little timeout
		while(--tm && !(I2C1->SR1 & 0x402)); //check for AF and ADDR
		
		if(!tm) return 1; //timeout error
		
		if(I2C1->SR1 & 0x400){ //AF flag is set
			GPIOB->ODR |= (1 << 9);
			I2C1->SR1 &= ~(0x400); //reset AF bit
			return 10;
		}
		
		//reset addr bit
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
	}
	else if(iic == 2){
		I2C2->DR = ((address << 1) | RW);  
		
		uint32_t tm = 30000000; //little timeout
		while(--tm && !(I2C2->SR1 & 0x402)); //check for AF and ADDR
		
		if(!tm) return 1; //timeout error
		
		if(I2C2->SR1 & 0x400){ //AF flag is set
			GPIOB->ODR |= (1 << 9);
			I2C2->SR1 &= ~(0x400); //reset AF bit
			return 10;
		}
		//reset addr bit
		tmp = I2C2->SR1;
		tmp = I2C2->SR2;
	}
	
	return 0; //good to go
}
//

void i2c_write(uint8_t iic, uint8_t address, uint8_t data[])
{
	i2c_start(iic);
	uint8_t st = i2c_address(iic, address, 0); //0 for W
	
	if(st != 0)
	//later
	
	for(uint16_t i=0; data[i]; i++){
		i2c_data(iic, data[i]);
	}
}
//

void i2c_data(uint8_t iic, uint8_t data)
{
	if(iic == 1){
		while(!(I2C1->SR1 & (1 << 7)));
		
		I2C1->DR = data;
	}
	else if(iic == 2){
		while(!(I2C2->SR1 & (1 << 7)));
		
		I2C2->DR = data;
	}
}
//

void i2c_stop(uint8_t iic)
{
		uint32_t tmp;
		if(iic == 1){
		while(!(I2C1->SR1 & (1 << 7)));
		
		I2C1->CR1 |= (1 << 9); // set stop bit
	}
	else if(iic == 2){
		while(!(I2C2->SR1 & (1 << 7)));
		
		I2C2->CR1 |= (1 << 9); // set stop bit
	}
}
