/*
 * bme280.h
 *
 * Created: 15-12-2018 21:54:11
 *  Author: Andreas
 */ 


#ifndef BME280_H_
#define BME280_H_
#include "asf.h"
#include "spi_master.h"

uint8_t device_id;

int32_t t_fine;
uint8_t cal_data[42];

uint16_t T1;
int16_t T2,T3;
uint16_t P1;
int16_t P2,P3,P4,P5,P6,P7,P8,P9;
uint8_t H1,H3;
int16_t H2,H4,H5;
int8_t H6;

struct bme280_data 
{
	int32_t temperature;
	int32_t pressure;
	double humidity;
	double dew_point;
};

void bme280_spi_write(uint8_t *data,uint16_t length);
void bme280_spi_read(uint8_t *data,uint16_t length,uint8_t reg);

void bme280_read_temp(uint8_t *data);
void bme280_read_pres(uint8_t *data);
void bme280_read_hum(uint8_t *data);
void bme280_get_cal_data(void);
void bme280_calc_cal(void);

int32_t bme280_calc_temp(int32_t read_temp);
int32_t bme280_calc_pres(int32_t read_pres);
double bme280_calc_hum(int32_t read_hum);
double bme280_calc_dew(int32_t temp, double hum);

void bme280_get_all_calced(struct bme280_data *bme280_data_object);

void bme280_force_update(void);
void bme280_init(void);

#endif /* BME280_H_ */