/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "bme280.h"



uint8_t device_id = 0;

uint8_t temps[3];
uint8_t pres[3];
uint8_t hum[2];

double temp_calced;
double pres_calced;
double hum_calced;


int main (void)
{
	system_init();
	delay_init();
	spi_master_init();
	
	uint8_t read_data[8];
	for (int i=0;i<7;i++)
	{
		read_data[i] = 0x00;
	}
	
	//bme280_spi_write(0xB6,1);
	
	
	delay_ms(10);
	bme280_get_cal_data();
	bme280_calc_cal();
	
	
	bme280_spi_read(&device_id,1,0xD0);
	uint8_t data[2];

	
	uint8_t temp =  0;
	bme280_spi_read(&temp,1,0xF2);
	
	data[0] = 0xF2;
	data[1] = 0x01;//temp & 0x7D;//
	bme280_spi_write(&data,2);
	data[0] = 0xF4;
	//data[1] = 0x25;
	data[1] = 0x26;//0b00100101;
	bme280_spi_write(&data,2);
	
	data[0] = 0xF5;
	data[1] = 0xC0;
	bme280_spi_write(&data,2);

	//bme280_spi_read(&read_data[7],1,0xF3);
	//bme280_spi_read(&read_data[6],1,0xF4);
	delay_ms(10);
	//bme280_spi_write(&data,2);
	
	
	/*bme280_spi_read(&read_data[0],1,0xFA);
	bme280_spi_read(&read_data[1],1,0xFB);
	bme280_spi_read(&read_data[2],1,0xFC);
	bme280_spi_read(&read_data[3],1,0xF7);
	bme280_spi_read(&read_data[4],1,0xF8);
	bme280_spi_read(&read_data[5],1,0xF9);*/
	
	
	while (1)
	{
		delay_ms(500);
		
		bme280_read_temp(&temps);
		bme280_read_pres(&pres);
		bme280_read_hum(&hum);
		temp_calced = bme280_calc_temp((int32_t)(temps[0]<<12)+(temps[1] << 4)+(temps[2] >> 4));
		pres_calced = bme280_calc_pres((int32_t)(pres[0]<<12)+(pres[1] << 4)+(pres[2] >> 4))/100;
		hum_calced = bme280_calc_hum((int32_t)(hum[0]<<8)+hum[1]);
		/*bme280_spi_read(&read_data[0],1,0xFA);
		bme280_spi_read(&read_data[1],1,0xFB);
		bme280_spi_read(&read_data[2],1,0xFC);
		bme280_spi_read(&read_data[3],1,0xF7);
		bme280_spi_read(&read_data[4],1,0xF8);
		bme280_spi_read(&read_data[5],1,0xF9);*/
	}

	/* Insert application code here, after the board has been initialized. */
}






