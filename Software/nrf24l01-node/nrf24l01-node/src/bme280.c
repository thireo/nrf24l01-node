/*
 * bme280.c
 *
 * Created: 15-12-2018 21:53:44
 *  Author: Andreas
 */ 

#include "bme280.h"
#include "math.h"



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

int32_t bme280_calc_temp(int32_t read_temp)
{
	int32_t var1, var2, T;
	var1 = ((((read_temp>>3) - ((int32_t)T1<<1))) * ((int32_t)T2)) >> 11;
	var2 = (((((read_temp>>4) - ((int32_t)T1)) * ((read_temp>>4) - ((int32_t)T1))) >> 12) *	((int32_t)T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

int32_t bme280_calc_pres(int32_t read_pres)
{
	int32_t var1, var2;
	uint32_t p;
	var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)P6);
	var2 = var2 + ((var1*((int32_t)P5))<<1);
	var2 = (var2>>2)+(((int32_t)P4)<<16);
	var1 = (((P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)P2) * var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)P1))>>15);
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-read_pres)-(var2>>12)))*3125;
	if (p < 0x80000000)
	{
		p = (p << 1) / ((uint32_t)var1);
	}
	else
	{
		p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)P8))>>13;
	p = (uint32_t)((int32_t)p + ((var1 + var2 + P7) >> 4));
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

double bme280_calc_dew(int32_t temp, double hum)
{
	double celsius = (double)temp/100;
	double RATIO = 373.15 / (273.15 + celsius);
	double RHS = -7.90298 * (RATIO - 1);
	RHS += 5.02808 * log10(RATIO);
	RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1/RATIO ))) - 1) ;
	RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
	RHS += log10(1013.246);
	// factor -3 is to adjust units - Vapor Pressure SVP * humidity
	double VP = pow(10, RHS - 3) * hum;
	// (2) DEWPOINT = F(Vapor Pressure)
	double T = log(VP/0.61078);   // temp var
	return (241.88 * T) / (17.558 - T);
}

void bme280_get_all_calced(struct bme280_data *bme280_data_object)
{
	uint8_t temps[3];
	uint8_t pres[3];
	uint8_t hum[2];
	
	bme280_force_update();
		
	bme280_read_temp(&temps);
	bme280_read_pres(&pres);
	bme280_read_hum(&hum);
	
	bme280_data_object->temperature = bme280_calc_temp((int32_t)(temps[0]<<12)+(temps[1] << 4)+(temps[2] >> 4));
	bme280_data_object->pressure = bme280_calc_pres((int32_t)(pres[0]<<12)+(pres[1] << 4)+(pres[2] >> 4))/100;
	bme280_data_object->humidity = (float)bme280_calc_hum((hum[0]<<8)+hum[1]);
	bme280_data_object->dew_point = (float)bme280_calc_dew(bme280_data_object->temperature, bme280_data_object->humidity);
}

void bme280_force_update(void)
{
	uint8_t data[2];
	data[0] = 0xF4;
	data[1] = 0x26;
	bme280_spi_write(&data,2);
	delay_ms(15);
}

void bme280_init(void)
{
	bme280_get_cal_data();
	bme280_calc_cal();
	bme280_spi_read(&device_id,1,0xD0);
	uint8_t data[2];
	
	uint8_t temp =  0;
	bme280_spi_read(&temp,1,0xF2);
	
	data[0] = 0xF2;
	data[1] = 0x01;
	bme280_spi_write(&data,2);
	data[0] = 0xF4;
	data[1] = 0x26;
	bme280_spi_write(&data,2);
	
	data[0] = 0xF5;
	data[1] = 0xC0;
	bme280_spi_write(&data,2);
}

union {
	float from;
	uint8_t bytes[4];
	}tobytearray;
	
typedef union {
	struct {
		int32_t temperature;
		int32_t pressure;
		float humidity;
		float dew_point;
		}hestestruct;
		uint8_t bytes[16];
	}testheste;

void bme280_data_compress(struct bme280_data *bme280_data_object, uint8_t *data_array)
{
	testheste testhest;
	testhest.hestestruct.temperature = bme280_data_object->temperature;
	testhest.hestestruct.pressure = bme280_data_object->pressure;
	testhest.hestestruct.dew_point = bme280_data_object->dew_point;
	testhest.hestestruct.humidity = bme280_data_object->humidity;
	
	for (int i=0;i<16;i++)
	{
		data_array[i] = testhest.bytes[i];
	}
	
	
	
	/*
	data_array[0] = (bme280_data_object->temperature & 0xFF);
	data_array[1] = ((bme280_data_object->temperature >> 8) & 0xFF);
	data_array[2] = ((bme280_data_object->temperature >> 16) & 0xFF);
	data_array[3] = ((bme280_data_object->temperature >> 24) & 0xFF);
	
	data_array[4] = (bme280_data_object->pressure & 0xFF);
	data_array[5] = ((bme280_data_object->pressure >> 8) & 0xFF);
	data_array[6] = ((bme280_data_object->pressure >> 16) & 0xFF);
	data_array[7] = ((bme280_data_object->pressure >> 24) & 0xFF);
	
	tobytearray.from = (float) bme280_data_object->dew_point;
		
	data_array[8] = tobytearray.bytes[0];
	data_array[9] = tobytearray.bytes[1];
	data_array[10] = tobytearray.bytes[2];
	data_array[11] = tobytearray.bytes[3];
	
	tobytearray.from = (float) bme280_data_object->humidity;
	
	data_array[12] = tobytearray.bytes[0];
	data_array[13] = tobytearray.bytes[1];
	data_array[14] = tobytearray.bytes[2];
	data_array[15] = tobytearray.bytes[3];*/
}