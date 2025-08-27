#include "ssd1306_drive.h"


void ssd1306_init(uint8_t iic)
{
	i2c_init(iic, I2C_SM);
	
	uint8_t cmd1[] = {0xA8,0x3F};
	ssd1306_cmd_2byte(iic, cmd1);
	
	uint8_t cmd2[] = {0xD3,0x00};
	ssd1306_cmd_2byte(iic, cmd2);
	
	ssd1306_cmd_byte(iic, 0x40);
	ssd1306_cmd_byte(iic, 0xA1);
	ssd1306_cmd_byte(iic, 0xC8);
	
	uint8_t cmd3[] = {0xDA,0x12};
	ssd1306_cmd_2byte(iic, cmd3);
	
	uint8_t cmd4[] = {0x81,0x7F};
	ssd1306_cmd_2byte(iic, cmd4);
	
	ssd1306_cmd_byte(iic, 0xA4);
	ssd1306_cmd_byte(iic, 0xA6);
	
	uint8_t cmd5[] = {0xD5,0x80};
	ssd1306_cmd_2byte(iic, cmd5);
	
	uint8_t cmd6[] = {0x8D,0x14};
	ssd1306_cmd_2byte(iic, cmd6);
	
	ssd1306_cmd_byte(iic, 0xAF);
	
	uint8_t cmd7[] = {0x20,0x10}; //setting memory address
	ssd1306_cmd_2byte(iic, cmd7);
}
//

void ssd1306_cmd_byte(uint8_t iic, uint8_t data)
{
	i2c_start(iic);
	uint8_t st = i2c_address(iic, PRIMARY_ADDRESS, 0); //0 for W
	
	if(st != 0){
		GPIOB->BSRR = (1 << 9);
		return;
	}
	
	i2c_data(iic, 0x00); //sending command
	i2c_data(iic, data);
	
	i2c_stop(iic);
}
//

void ssd1306_cmd_2byte(uint8_t iic, uint8_t data[])
{
	i2c_start(iic);
	uint8_t st = i2c_address(iic, PRIMARY_ADDRESS, 0); //0 for W
	
	if(st != 0) return;
	
	i2c_data(iic, 0x00); //sending command
	
	i2c_data(iic, data[0]);
	i2c_data(iic, data[1]);
	
	i2c_stop(iic);
}
//

void ssd1306_data_byte(uint8_t iic, uint8_t data)
{
	i2c_start(iic);
	uint8_t st = i2c_address(iic, PRIMARY_ADDRESS, 0); //0 for W
	
	if(st != 0) return;
	
	i2c_data(iic, 0x40); //data control byte
	i2c_data(iic, data);
	
	i2c_stop(iic);
}
//

void ssd1306_setpos(uint8_t iic, uint8_t Page, uint8_t Coll)
{
	ssd1306_cmd_byte(iic, (0xB0 + Page)); //set page
	ssd1306_cmd_byte(iic, (0x00 + (Coll & 0x0F))); //lower nibble?
	ssd1306_cmd_byte(iic, (0x10 + (Coll >> 4))); //upper nibble?
}
//

void ssd1306_data_start(uint8_t iic)
{
	i2c_start(iic);
	uint8_t st = i2c_address(iic, PRIMARY_ADDRESS, 0); //0 for W
	
	if(st != 0) return;
	
	i2c_data(iic, 0x40); //data control byte
}
//

void ssd1306_data(uint8_t iic, uint8_t data)
{
	i2c_data(iic, data);
}
//

void ssd1306_data_stop(uint8_t iic)
{
	i2c_stop(iic);
}
//

void ssd1306_print(uint8_t iic, const char msg[])
{	
	ssd1306_data_start(iic);
	
	for(uint16_t i=0; msg[i]; i++) 
		for(uint8_t j=0; j<5; j++) 
			ssd1306_data(iic, (ASCII_5[msg[i]-32][j]));
	
	ssd1306_data_stop(iic);
}
//

void ssd1306_print_pos(uint8_t iic, const char msg[], uint8_t Page, uint8_t Coll)
{	
	ssd1306_setpos(iic, Page, Coll);
	
	ssd1306_data_start(iic);
	for(uint16_t i=0; msg[i]; i++)
	{
		ssd1306_data(iic, 0b00);
		for(uint8_t j=0; j<5; j++)
			ssd1306_data(iic, (ASCII_5[msg[i]-32][j]));
	}
	
	ssd1306_data_stop(iic);
}
//

void ssd1306_blank(uint8_t iic)
{
	for(uint8_t i=0;i<8;i++){
		ssd1306_setpos(iic, i, 0x00);
		ssd1306_data_start(iic);
		for(uint8_t j=0;j<128;j++){
			ssd1306_data(iic, 0x00);
		}
		ssd1306_data_stop(iic);
	}
	
	ssd1306_setpos(iic, 0x00, 0x00);
}
//

