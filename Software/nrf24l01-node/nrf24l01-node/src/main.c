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


struct bme280_data bme280_data_object;

int main (void)
{
	system_init();
	delay_init();
	spi_master_init();
	
	bme280_init();
	//delay_ms(10);
	
	
	
	
	while (1)
	{
		delay_ms(500);
		bme280_get_all_calced(&bme280_data_object);
	}

	/* Insert application code here, after the board has been initialized. */
}






