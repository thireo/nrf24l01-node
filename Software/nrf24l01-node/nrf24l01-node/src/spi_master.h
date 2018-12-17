/*
 * spi_master.h
 *
 * Created: 15-12-2018 21:55:20
 *  Author: Andreas
 */ 


#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_
#include "asf.h"

#define SPI_MODULE SERCOM0
#define SPI_MOSI PINMUX_PA08C_SERCOM0_PAD0
#define SPI_SCK PINMUX_PA09C_SERCOM0_PAD1
#define SPI_MISO PINMUX_PA11C_SERCOM0_PAD3
#define SPI_MUX_SETTING SPI_SIGNAL_MUX_SETTING_D

#define BME280_CE PIN_PA10
#define NRF24L01_CE PIN_PA12

struct spi_module spi_master_instance;
struct spi_slave_inst bme280_slave_instance;
struct spi_slave_inst nrf14l01_slave_instance;


void spi_master_init(void);




#endif /* SPI_MASTER_H_ */