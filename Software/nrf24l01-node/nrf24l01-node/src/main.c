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
#include "nrf24l01.h"


struct bme280_data bme280_data_object;
char RX_ADDR[5] = {0x42,0x42,0x42,0x42,0};
char TX_ADDR[5] = {0x2A,0x2A,0x2A,0x2A,0};
	
uint8_t data_array[16];

int main (void)
{
	system_init();
	delay_init();
	spi_master_init();
	
	bme280_init();
	//nrf24l01_init();
	
	
	//delay_ms(10);
	
	NRF_Enter_TX_Mode(TX_ADDR,RX_ADDR);
	
	
	while (1)
	{
		bme280_get_all_calced(&bme280_data_object);
		bme280_data_compress(&bme280_data_object,&data_array);
		uint32_t status = NRF_TX_data(&data_array,16);
		//uint32_t status = NRF_TX(NRF_CONTACT_2BTN_V1,NRF_CMD_OFF);
		delay_ms(100);
	}

	/* Insert application code here, after the board has been initialized. */
}






