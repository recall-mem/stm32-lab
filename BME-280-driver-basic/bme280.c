#include "bme280.h"


#define bme280_ADDR1 0x77 // SDO ~ VCC
#define bme280_ADDR2 0x76	// SDO ~ GND 

#define bme280_ADDR bme280_ADDR1

// Status and ID registers
#define bme280_REG_ID          0xD0  // Chip ID - should read 0x58
//#define bme280_REG_RESET       0xE0  // Soft reset register (write only)
#define bme280_REG_STATUS      0xF3  // Status register
//#define bme280_REG_CTRL_MEAS   0xF4  // Control measurement register
//#define bme280_REG_CTRL_HUM		 0xF2  // Control humidity register
//#define bme280_REG_CONFIG      0xF5  // Configuration register (R/W)

// Data registers (READ only)
#define bme280_REG_PRESS_MSB   0xF7  // Pressure MSB
//#define bme280_REG_PRESS_LSB   0xF8  // Pressure LSB  
//#define bme280_REG_PRESS_XLSB  0xF9  // Pressure XLSB (bits 7:4)
//#define bme280_REG_TEMP_MSB    0xFA  // Temperature MSB
//#define bme280_REG_TEMP_LSB    0xFB  // Temperature LSB
//#define bme280_REG_TEMP_XLSB   0xFC  // Temperature XLSB (bits 7:4)
//#define bme280_REG_RH_MSB      0xFD  // Humidity MSB
//#define bme280_REG_RH_LSB      0xFE  // Humidity LSB

// Calibration registers read only (total 32 bytes)
#define bme280_REG_CALIB_TP_START  0x88  // Start of temp/pressure calibration registers
//#define bme280_REG_CALIB_TP_END    0x9F  // End of temp/pressure calibration rehisters (24 bytes)
#define bme280_REG_CALIB_H1        0xA1  // Humidity calibration H1 (1 byte)
#define bme280_REG_CALIB_H2_START  0xE1  // Start of humidity calibration H2-H6
//#define bme280_REG_CALIB_H2_END    0xE7  //  End of humidity calibration (7 bytes)


#define R_OP 1
#define W_OP 0

#define ACK_SET 	1
#define ACK_RESET 0


bme280_calib_data calib_vals;
int32_t t_fine;



void bme280_init_normal()
{

}



uint8_t bme280_get_device_id()
{
	uint8_t id;
	
	generate_start();
	
	//change address if SDA pin have pull up resistor
	send_address(bme280_ADDR, W_OP); 
	transfer_byte(bme280_REG_ID);
		
	generate_start();
	send_address(bme280_ADDR, R_OP); 
	
	id = read_byte(ACK_RESET);
	generate_stop();
	
	return id;
}



uint8_t bme280_status()
{
	//status register 0xF3
	//Bit 3 "measuring" state automatically set to '1' if conversion is
	//running, '0' when the results have been transferred to the data registers.
	
	//Bit 0 "im_update" Automatically set to '1' when the NVM data are being
	//copied to image registers '0' when the copying is done.
	
	uint8_t status;
	
	generate_start();
	
	send_address(bme280_ADDR, W_OP);
	transfer_byte(bme280_REG_STATUS);
	
	generate_start();
	
	send_address(bme280_ADDR, R_OP);
	status = read_byte(ACK_RESET);
	
	generate_stop();
	
	
	return status;
}



void bme280_get_calibration_values()
{
	uint8_t BUFFER[32];
	
	
	//Reading Temp/Press calibration registers first
	generate_start();
	
	send_address(bme280_ADDR, W_OP);
	transfer_byte(bme280_REG_CALIB_TP_START);
	
	generate_start();
	
	send_address(bme280_ADDR, R_OP);
	read_bytes(BUFFER, 24);
	
	
	//Reading Humidity calibration registers
	//read the fist humidity calibration byte 
	generate_start();
	
	send_address(bme280_ADDR, W_OP);
	transfer_byte(bme280_REG_CALIB_H1); 
	
	generate_start();
	
	send_address(bme280_ADDR, R_OP);
	BUFFER[24] = read_byte(ACK_RESET);
	
	//read the remaining 7 humidity calibration bytes
	generate_start();
	
	send_address(bme280_ADDR, W_OP);
	transfer_byte(bme280_REG_CALIB_H2_START); 
	
	generate_start();
	
	send_address(bme280_ADDR, R_OP);
	read_bytes((BUFFER + 25), 7); 
	
	generate_stop();
	
	bme280_parse_calibration_values(BUFFER);
}



void bme280_parse_calibration_values(uint8_t buf[])
{
	//Temprature
	calib_vals.dig_T1 = (uint16_t)(buf[1] << 8 | buf[0]);
	calib_vals.dig_T2 = (int16_t)(buf[3] << 8 | buf[2]);
	calib_vals.dig_T3 = (int16_t)(buf[5] << 8 | buf[4]);
	
	//Pressure
	calib_vals.dig_P1 = (uint16_t)(buf[7] << 8 | buf[6]);
	calib_vals.dig_P2 = (int16_t)(buf[9] << 8 | buf[8]);
	calib_vals.dig_P3 = (int16_t)(buf[11] << 8 | buf[10]);
	calib_vals.dig_P4 = (int16_t)(buf[13] << 8 | buf[12]);
	calib_vals.dig_P5 = (int16_t)(buf[15] << 8 | buf[14]);
	calib_vals.dig_P6 = (int16_t)(buf[17] << 8 | buf[16]);
	calib_vals.dig_P7 = (int16_t)(buf[19] << 8 | buf[18]);
	calib_vals.dig_P8 = (int16_t)(buf[21] << 8 | buf[20]);
	calib_vals.dig_P9 = (int16_t)(buf[23] << 8 | buf[22]);

	//Humidity
	calib_vals.dig_H1 = buf[24]; // H1 

	calib_vals.dig_H2 = (int16_t)(buf[26] << 8 | buf[25]);
	calib_vals.dig_H3 = buf[27];

	//H4 and H5 are packed
	//H4 = (buf[28] << 4) | (buf[29] & 0x0F)
	//H5 = (buf[30] << 4) | (buf[29] >> 4)
	calib_vals.dig_H4 = (int16_t)((buf[28] << 4) | (buf[29] & 0x0F));
	calib_vals.dig_H5 = (int16_t)((buf[30] << 4) | (buf[29] >> 4));

	calib_vals.dig_H6 = (int8_t)buf[31];
}



void bme280_write_register(uint8_t reg, uint8_t setting)
{
	generate_start();
	
	send_address(bme280_ADDR, W_OP); 
	
	transfer_byte(reg);
	transfer_byte(setting);
	
	generate_stop();
}



bme280_reads_raw bme280_get_data_raw()
{
	uint8_t TEMP_BUF[8];
	bme280_reads_raw raw_reads;
	
	generate_start();
	
	send_address(bme280_ADDR, W_OP);
	transfer_byte(bme280_REG_PRESS_MSB);
	
	generate_start();
	
	send_address(bme280_ADDR, R_OP);
	read_bytes(TEMP_BUF, 8);
	
	generate_stop();
	
	//Temperature raw 20-bit
	raw_reads.adc_T = (uint32_t)((TEMP_BUF[3] << 12) | (TEMP_BUF[4] << 4) | (TEMP_BUF[5] >> 4));
	
	//Pressure raw 20-bit
	raw_reads.adc_P = (uint32_t)((TEMP_BUF[0] << 12) | (TEMP_BUF[1] << 4) | (TEMP_BUF[2] >> 4));

	//Humidity raw 16-bit
	raw_reads.adc_H = (uint32_t)((TEMP_BUF[6] << 8) | TEMP_BUF[7]);
	
	
	return raw_reads;
}



bme280_reads_compensated_float bme280_get_compensated_float(bme280_reads_raw raw_data)
{
	bme280_reads_compensated_float data;
	
	data.TEMP 		= bme280_compensate_temperature_float(raw_data.adc_T);
	data.PRESSURE = bme280_compensate_pressure_float(raw_data.adc_P);
	data.HUMIDITY = bme280_compensate_humidity_float(raw_data.adc_H);
	
	
	return data;
}



bme280_reads_compensated_int bme280_get_compensated_int(bme280_reads_raw raw_data)
{
	bme280_reads_compensated_int data;
	
	data.TEMP 		= bme280_compensate_temperature_int(raw_data.adc_T);
	data.PRESSURE = bme280_compensate_pressure_int(raw_data.adc_P);
	data.HUMIDITY = bme280_compensate_humidity_int(raw_data.adc_H);
	
	
	return data;
}



float bme280_compensate_temperature_float(int32_t adc_T)
{
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)calib_vals.dig_T1 << 1))) * ((int32_t)calib_vals.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)calib_vals.dig_T1)) * ((adc_T >> 4) - ((int32_t)calib_vals.dig_T1))) >> 12) *
            ((int32_t)calib_vals.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T / 100.0f; //Cels
}



float bme280_compensate_pressure_float(uint32_t adc_P)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib_vals.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib_vals.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib_vals.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib_vals.dig_P3) >> 8) +
           ((var1 * (int64_t)calib_vals.dig_P2) << 12);
    var1 = (((((int64_t)1 << 47) + var1)) * ((int64_t)calib_vals.dig_P1)) >> 33;

    if (var1 == 0) return 0; //avoid divide by zero

    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib_vals.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib_vals.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calib_vals.dig_P7) << 4);

    return p / 256.0f; //Pascals
}



float bme280_compensate_humidity_float(uint32_t adc_H)
{
    int32_t v_x1_u32r;

    v_x1_u32r = t_fine - 76800;
    v_x1_u32r = (((((adc_H << 14) - ((int32_t)calib_vals.dig_H4 << 20) - 
                    ((int32_t)calib_vals.dig_H5 * v_x1_u32r)) + 16384) >> 15) *
                 (((((((v_x1_u32r * (int32_t)calib_vals.dig_H6) >> 10) *
                      (((v_x1_u32r * (int32_t)calib_vals.dig_H3) >> 11) + 32768)) >> 10) + 2097152) *
                   (int32_t)calib_vals.dig_H2 + 8192) >> 14));
    v_x1_u32r = v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                              (int32_t)calib_vals.dig_H1) >> 4);
    if (v_x1_u32r < 0)
        v_x1_u32r = 0;
    if (v_x1_u32r > 419430400)
        v_x1_u32r = 419430400;
    return (float)(v_x1_u32r >> 12) / 1024.0f; //in %
}



int32_t bme280_compensate_temperature_int(int32_t adc_T)
{
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)calib_vals.dig_T1 << 1))) * ((int32_t)calib_vals.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)calib_vals.dig_T1)) * ((adc_T >> 4) - ((int32_t)calib_vals.dig_T1))) >> 12) *
            ((int32_t)calib_vals.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T; //in 0.01 °C
}



uint32_t bme280_compensate_pressure_int(uint32_t adc_P)
{
    int64_t var1, var2, p;
    var1 = (int64_t)t_fine - 128000;
    var2 = var1 * var1 * (int64_t)calib_vals.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib_vals.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib_vals.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib_vals.dig_P3) >> 8) +
           ((var1 * (int64_t)calib_vals.dig_P2) << 12);
    var1 = (((((int64_t)1 << 47) + var1)) * ((int64_t)calib_vals.dig_P1)) >> 33;

    if (var1 == 0) return 0; //avoid division by zero

    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib_vals.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib_vals.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calib_vals.dig_P7) << 4);

    return (int32_t)(p / 256); //in Pa
}




uint32_t bme280_compensate_humidity_int(uint32_t adc_H)
{
    int32_t v_x1_u32r;

    v_x1_u32r = t_fine - 76800;
    v_x1_u32r = (((((adc_H << 14) - ((int32_t)calib_vals.dig_H4 << 20) - 
                    ((int32_t)calib_vals.dig_H5 * v_x1_u32r)) + 16384) >> 15) *
                 (((((((v_x1_u32r * (int32_t)calib_vals.dig_H6) >> 10) *
                      (((v_x1_u32r * (int32_t)calib_vals.dig_H3) >> 11) + 32768)) >> 10) + 2097152) *
                   (int32_t)calib_vals.dig_H2 + 8192) >> 14));
    v_x1_u32r = v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                              (int32_t)calib_vals.dig_H1) >> 4);
    if (v_x1_u32r < 0)
        v_x1_u32r = 0;
    if (v_x1_u32r > 419430400)
        v_x1_u32r = 419430400;
    return (uint32_t)(v_x1_u32r >> 12); //in 0.001 % RH
}





