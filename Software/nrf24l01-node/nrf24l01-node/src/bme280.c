/*
 * bme280.c
 *
 * Created: 15-12-2018 21:53:44
 *  Author: Andreas
 */ 

#include "bme280.h"

//enum status_code bme280_spi_send(uint8_t *data)
void bme280_spi_write(uint8_t *data,uint16_t length)
{
	data[0] &= 0x7F;
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,true);
	spi_write_buffer_wait(&spi_master_instance,data,length);
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,false);
}

void bme280_spi_read(uint8_t *data,uint16_t length,uint8_t reg)
{
	uint16_t dummy = reg | 0x80;
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,true);
	spi_write_buffer_wait(&spi_master_instance,&dummy,1);
	spi_read_buffer_wait(&spi_master_instance,data,length,dummy);
	//spi_transceive_wait(&spi_master_instance,reg,data);
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,false);
}

void bme280_read_temp(uint8_t *data)
{
	uint16_t dummy = 0xFA | 0x80;
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,true);
	spi_write_buffer_wait(&spi_master_instance,&dummy,1);
	spi_read_buffer_wait(&spi_master_instance,data,3,0x00);
	//spi_transceive_wait(&spi_master_instance,reg,data);
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,false);
}

void bme280_read_pres(uint8_t *data)
{
	uint16_t dummy = 0xF7 | 0x80;
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,true);
	spi_write_buffer_wait(&spi_master_instance,&dummy,1);
	spi_read_buffer_wait(&spi_master_instance,data,3,0x00);
	//spi_transceive_wait(&spi_master_instance,reg,data);
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,false);
}

void bme280_read_hum(uint8_t *data)
{
	uint16_t dummy = 0xFD | 0x80;
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,true);
	spi_write_buffer_wait(&spi_master_instance,&dummy,1);
	spi_read_buffer_wait(&spi_master_instance,data,2,0x00);
	//spi_transceive_wait(&spi_master_instance,reg,data);
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,false);
}

void bme280_get_cal_data(void)
{
	//uint8_t cal_data[42];
	uint16_t reg = 0x88;
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,true);
	spi_write_buffer_wait(&spi_master_instance,&reg,1);
	spi_read_buffer_wait(&spi_master_instance,&cal_data,26,0x00);
	reg = 0xE1;
	spi_write_buffer_wait(&spi_master_instance,&reg,1);
	spi_read_buffer_wait(&spi_master_instance,&cal_data[26],7,0x00);
	spi_select_slave(&spi_master_instance,&bme280_slave_instance,false);
}

void bme280_calc_cal(void)
{
	T1 = (cal_data[1] << 8)+cal_data[0];
	T2 = (cal_data[3] << 8)+cal_data[2];
	T3 = (cal_data[5] << 8)+cal_data[4];
	
	P1 = (cal_data[7] << 8)+cal_data[6];
	P2 = (cal_data[9] << 8)+cal_data[8];
	P3 = (cal_data[11] << 8)+cal_data[10];
	P4 = (cal_data[13] << 8)+cal_data[12];
	P5 = (cal_data[15] << 8)+cal_data[14];
	P6 = (cal_data[17] << 8)+cal_data[16];
	P7 = (cal_data[19] << 8)+cal_data[18];
	P8 = (cal_data[21] << 8)+cal_data[20];
	P9 = (cal_data[23] << 8)+cal_data[22];
	
	H1 = cal_data[24];
	H2 = (cal_data[26] << 8)+cal_data[25];
	H3 = cal_data[27];
	H4 = (cal_data[29] << 4)+(cal_data[28] & 0x0F);
	H5 = (cal_data[31] << 4)+((cal_data[30] >> 4) & 0x0F);
	H6 = cal_data[32];
}

double bme280_calc_temp(int32_t read_temp)
{
	double var1, var2, T;
	var1 = (((double)read_temp)/16384.0 - ((double)T1)/1024.0) * ((double)T2);
	var2 = ((((double)read_temp)/131072.0 - ((double)T1)/8192.0) * (((double)read_temp)/131072.0 - ((double) T1)/8192.0)) * ((double)T3);
	t_fine = (int32_t)(var1 + var2);
	T = (var1 + var2) / 5120.0;
	return T;
}

double bme280_calc_pres(int32_t read_pres)
{
	double var1, var2, p;
	var1 = ((double)t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)P6) / 32768.0;
	var2 = var2 + var1 * ((double)P5) * 2.0;
	var2 = (var2/4.0)+(((double)P4) * 65536.0);
	var1 = (((double)P3) * var1 * var1 / 524288.0 + ((double)P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)P1);
	if (var1 == 0.0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576.0 - (double)read_pres;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)P9) * p * p / 2147483648.0;
	var2 = p * ((double)P8) / 32768.0;
	p = p + (var1 + var2 + ((double)P7)) / 16.0;
	return p;
}

double bme280_calc_hum(int32_t read_hum)
{
	double var_H;
	var_H = (((double)t_fine) - 76800.0);
	var_H = (read_hum - (((double)H4) * 64.0 + ((double)H5) / 16384.0 * var_H)) * (((double)H2) / 65536.0 * (1.0 + ((double)H6) / 67108864.0 * var_H * (1.0 + ((double)H3) / 67108864.0 * var_H)));
	var_H = var_H * (1.0 - ((double)H1) * var_H / 524288.0);
	if (var_H > 100.0)
		var_H = 100.0;
	else if (var_H < 0.0)
		var_H = 0.0;
	return var_H;
}