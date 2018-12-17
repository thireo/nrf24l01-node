/*
 * spi_master.c
 *
 * Created: 15-12-2018 21:55:37
 *  Author: Andreas
 */ 

#include "spi_master.h"

void spi_master_init(void)
{
	struct spi_config config_spi;
	struct spi_slave_inst_config bme280_slave_config;
	struct spi_slave_inst_config nrf24l01_slave_config;
	
	spi_slave_inst_get_config_defaults(&bme280_slave_config);
	bme280_slave_config.ss_pin = BME280_CE;
	spi_attach_slave(&bme280_slave_instance, &bme280_slave_config);
	
	spi_slave_inst_get_config_defaults(&nrf24l01_slave_config);
	nrf24l01_slave_config.ss_pin = NRF24L01_CE;
	spi_attach_slave(&nrf14l01_slave_instance, &nrf24l01_slave_config);
	
	
	/* Configure, initialize and enable SERCOM SPI module */
	spi_get_config_defaults(&config_spi);
	config_spi.mux_setting = SPI_MUX_SETTING;
	/* Configure pad 0 for data in */
	config_spi.pinmux_pad0 = SPI_MOSI;
	/* Configure pad 1 as unused */
	config_spi.pinmux_pad1 = SPI_SCK;
	/* Configure pad 2 for data out */
	config_spi.pinmux_pad2 = PINMUX_UNUSED;
	/* Configure pad 3 for SCK */
	config_spi.pinmux_pad3 = SPI_MISO;
	config_spi.receiver_enable = true;
	spi_init(&spi_master_instance, SPI_MODULE, &config_spi);
	spi_enable(&spi_master_instance);
}

