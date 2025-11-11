#ifndef bme280_H
#define bme280_H


#include "stdint.h"
#include "i2c.h"



typedef struct {
	int32_t adc_T;
	uint32_t adc_P;
	uint32_t adc_H;
} bme280_reads_raw;


typedef struct {
	float TEMP;
	float PRESSURE;
	float HUMIDITY;
} bme280_reads_compensated_float;


typedef struct {
	int32_t TEMP;
	uint32_t PRESSURE;
	uint32_t HUMIDITY;
} bme280_reads_compensated_int;


typedef struct {
    uint16_t dig_T1;  // 0x88-0x89 temperature calibration
    int16_t  dig_T2;  // 0x8A-0x8B
    int16_t  dig_T3;  // 0x8C-0x8D

    uint16_t dig_P1;  // 0x8E-0x8F pressure calibration
    int16_t  dig_P2;  // 0x90-0x91
    int16_t  dig_P3;  // 0x92-0x93
    int16_t  dig_P4;  // 0x94-0x95
    int16_t  dig_P5;  // 0x96-0x97
    int16_t  dig_P6;  // 0x98-0x99 
    int16_t  dig_P7;  // 0x9A-0x9B
    int16_t  dig_P8;  // 0x9C-0x9D
    int16_t  dig_P9;  // 0x9E-0x9F

    uint8_t  dig_H1;  // 0xA1 humidity calibration
    int16_t  dig_H2;  // 0xE1-0xE2
    uint8_t  dig_H3;  // 0xE3
    int16_t  dig_H4;  // 0xE4-0xE5 (upper/lower nibbles packed)
    int16_t  dig_H5;  // 0xE5-0xE6 (upper/lower nibbles packed)
    int8_t   dig_H6;  // 0xE7
} bme280_calib_data;



extern bme280_calib_data calib_vals;


void bme280_init_normal();
uint8_t bme280_status();
uint8_t bme280_get_device_id(); 
void bme280_get_calibration_values();
void bme280_parse_calibration_values(uint8_t buf[]);

void bme280_write_register(uint8_t reg, uint8_t setting);
bme280_reads_raw bme280_get_data_raw();

bme280_reads_compensated_float bme280_get_compensated_float(bme280_reads_raw raw_data);
bme280_reads_compensated_int bme280_get_compensated_int(bme280_reads_raw raw_data);

float bme280_compensate_temperature_float(int32_t adc_T);
float bme280_compensate_pressure_float(uint32_t adc_P);
float bme280_compensate_humidity_float(uint32_t adc_H);

int32_t bme280_compensate_temperature_int(int32_t adc_T);
uint32_t bme280_compensate_pressure_int(uint32_t adc_P);
uint32_t bme280_compensate_humidity_int(uint32_t adc_H);



#endif