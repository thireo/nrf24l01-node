/*
 * nrf24l01.h
 *
 * Created: 18-12-2018 20:53:21
 *  Author: Andreas
 */ 


#ifndef NRF24L01_H_
#define NRF24L01_H_

#include "asf.h"
#include "spi_master.h"
#include "main.h"
#include "uart.h"


#define NRF24L01_IRQ PIN_PA13
#define NRF24L01_CS PIN_PA12
#define NRF24L01_CE PIN_PA10

#define NRF_PACKAGE_LENGTH 4

///////////////////////////////////////////////////
///////////				TYPES			///////////
///////////////////////////////////////////////////
#define NRF_HUB_V1 1
#define NRF_CONTACT_2BTN_V1 2
#define NRF_LED_STRIP_V1 3


///////////////////////////////////////////////////
///////////			Commands			///////////
///////////////////////////////////////////////////

#define NRF_CMD_BTN1 0x50
#define NRF_CMD_BTN2 0x51

#define NRF_CMD_OFF 100
#define NRF_CMD_ON	200


uint32_t nrf24l01_spi_write(uint8_t *data,uint16_t length);
uint32_t nrf24l01_spi_write_single(uint8_t *data);
void nrf24l01_cmd2(uint32_t cmd, uint8_t length, uint8_t data_input);
void nrf24l01_cmd(uint32_t cmd, uint8_t data_input);

void nrf24l01_spi_read(uint8_t *data,uint16_t length,uint16_t reg);
void nrf24l01_init(void);

uint32_t NRF_TX(uint32_t Type, uint32_t Command);
uint32_t NRF_TX_data(uint8_t *data, uint32_t length);

void NRF_Enter_TX_Mode(char To_Addr[],char From_Addr[]);
void NRF_Enter_RX_Mode(char Addr[]);

uint32_t NRF_SPI_RW(uint32_t input);

uint32_t NRF_Read(uint32_t Command, uint32_t Length, char *buffer);
uint32_t NRF_Read_single(uint32_t Command);

uint32_t NRF_Write(uint32_t Command, uint32_t Length, char *Input);
uint32_t NRF_Write_2(uint32_t Command, char Input);
uint32_t NRF_Write_1(uint32_t Command);

nrf_enable_vcc(void);
nrf_disable_vcc(void);

void nrf24l01_status(void);

#define NRF_FAILURE 0
#define NRF_SUCCES 1

#define NRF_R_REGISTER(x)		(x & 0b00011111)
#define NRF_W_REGISTER(x)		((x & 0b00011111) | 0b00100000)
#define NRF_R_RX_PAYLOAD		0b01100001
#define NRF_W_TX_PAYLOAD		0b10100000
#define NRF_FLUSH_TX			0b11100001
#define NRF_FLUSH_RX			0b11100010
#define NRF_REUSE_TX_PL			0b11100011
#define NRF_R_RX_PL_WID			0b01100000
#define NRF_W_ACK_PAYLOAD(x)	((x & 0b00000111 ) | 0b10101000)
#define NRF_W_TX_PAYLOAD_NOACK	0b10110000
#define NRF_NOP					0b11111111

#define NRF_CONFIG_REG				0x00
#define NRF_CONFIG_PRIM_RX			(1u<<0)
#define NRF_CONFIG_PRIM_TX			(0u<<0)
#define NRF_CONFIG_PWR_UP			(1u<<1)
#define NRF_CONFIG_PWR_DOWN			(0u<<1)
#define NRF_CONFIG_CRCO_2BYTE		(1u<<2)
#define NRF_CONFIG_CRCO_1BYTE		(0u<<2)
#define NRF_CONFIG_EN_CRC			(1u<<3)
#define NRF_CONFIG_MASK_MAX_RT		(1u<<4)
#define NRF_EN_AA_REG				0x01
#define NRF_EN_AA_ENAA_P5			(1u<<5)
#define NRF_EN_AA_ENAA_P4			(1u<<4)
#define NRF_EN_AA_ENAA_P3			(1u<<3)
#define NRF_EN_AA_ENAA_P2			(1u<<2)
#define NRF_EN_AA_ENAA_P1			(1u<<1)
#define NRF_EN_AA_ENAA_P0			(1u<<0)
#define NRF_EN_RXADDR_REG			0x02
#define NRF_EN_RXADDR_ERX_P0		(1u<<0)
#define NRF_EN_RXADDR_ERX_P1		(1u<<1)
#define NRF_EN_RXADDR_ERX_P2		(1u<<2)
#define NRF_EN_RXADDR_ERX_P3		(1u<<3)
#define NRF_EN_RXADDR_ERX_P4		(1u<<4)
#define NRF_EN_RXADDR_ERX_P5		(1u<<5)
#define NRF_SETUP_AW_REG			0x03
#define NRF_SETUP_AW_3BYTES			(1u<<0)
#define NRF_SETUP_AW_4BYTES			(2u<<0)
#define NRF_SETUP_AW_5BYTES			(3u<<0)
#define NRF_SETUP_RETR_REG			0x04
#define NRF_SETUP_RETR_ARD_W250		(0<<4)
#define NRF_SETUP_RETR_ARD_W500		(1<<4)
#define NRF_SETUP_RETR_ARD_W750		(2<<4)
#define NRF_SETUP_RETR_ARD_W1000	(3<<4)
#define NRF_SETUP_RETR_ARD_W1250	(4<<4)
#define NRF_SETUP_RETR_ARD_W1500	(5<<4)
#define NRF_SETUP_RETR_ARD_W1750	(6<<4)
#define NRF_SETUP_RETR_ARD_W2000	(7<<4)
#define NRF_SETUP_RETR_ARD_W2250	(8<<4)
#define NRF_SETUP_RETR_ARD_W2500	(9<<4)
#define NRF_SETUP_RETR_ARD_W2750	(10<<4)
#define NRF_SETUP_RETR_ARD_W3000	(11<<4)
#define NRF_SETUP_RETR_ARD_W3250	(12<<4)
#define NRF_SETUP_RETR_ARD_W3500	(13<<4)
#define NRF_SETUP_RETR_ARD_W3750	(14<<4)
#define NRF_SETUP_RETR_ARD_W4000	(15<<4)
#define NRF_SETUP_RETR_ARC_DISABLE	(0<<0)
#define NRF_SETUP_RETR_ARC_RT1		(1<<0)
#define NRF_SETUP_RETR_ARC_RT2		(2<<0)
#define NRF_SETUP_RETR_ARC_RT3		(3<<0)
#define NRF_SETUP_RETR_ARC_RT4		(4<<0)
#define NRF_SETUP_RETR_ARC_RT5		(5<<0)
#define NRF_SETUP_RETR_ARC_RT6		(6<<0)
#define NRF_SETUP_RETR_ARC_RT7		(7<<0)
#define NRF_SETUP_RETR_ARC_RT8		(8<<0)
#define NRF_SETUP_RETR_ARC_RT9		(9<<0)
#define NRF_SETUP_RETR_ARC_RT10		(10<<0)
#define NRF_SETUP_RETR_ARC_RT11		(11<<0)
#define NRF_SETUP_RETR_ARC_RT12		(12<<0)
#define NRF_SETUP_RETR_ARC_RT13		(13<<0)
#define NRF_SETUP_RETR_ARC_RT14		(14<<0)
#define NRF_SETUP_RETR_ARC_RT15		(15<<0)
#define NRF_RF_CH_REG				0x05
#define NRF_RF_SETUP_REG			0x06
#define NRF_RF_SETUP_CONT_WAVE		(1<<7)
#define NRF_RF_SETUP_RF_DR_LOW		(1<<5)
#define NRF_RF_SETUP_PLL_LOCK		(1<<4)
#define NRF_RF_SETUP_RF_DR_HIGH		(1<<3)
#define NRF_RF_SETUP_DR_1MBPS 0
#define NRF_RF_SETUP_DR_2MBPS NRF_RF_SETUP_RF_DR_HIGH
#define NRF_RF_SETUP_250KBPS NRF_RF_SETUP_RF_DR_LOW
#define NRF_RF_SETUP_RF_PWR_18DBM	(0<<1)
#define NRF_RF_SETUP_RF_PWR_12DBM	(1<<1)
#define NRF_RF_SETUP_RF_PWR_6DBM	(2<<1)
#define NRF_RF_SETUP_RF_PWR_0DBM	(3<<1)
#define NRF_STATUS_REG				0x07
#define NRF_STATUS_TX_FULL			(1u<<0)
#define NRF_STATUS_RX_PIPE0			(0b000<<1)
#define NRF_STATUS_RX_PIPE1			(0b001<<1)
#define NRF_STATUS_RX_PIPE2			(0b010<<1)
#define NRF_STATUS_RX_PIPE3			(0b011<<1)
#define NRF_STATUS_RX_PIPE4			(0b100<<1)
#define NRF_STATUS_RX_PIPE5			(0b101<<1)
#define NRF_STATUS_RX_FIFO_EMPTY	(0b111<<1)
#define NRF_STATUS_MAX_RT			(1u<<4)
#define NRF_STATUS_TX_DS			(1u<<5)
#define NRF_STATUS_RX_DR			(1u<<6)


#define NRF_RPD_REG					0x09
#define NRF_RX_ADDR_P0_REG			0x0A
#define NRF_RX_ADDR_P1_REG			0x0B
#define NRF_RX_ADDR_P2_REG			0x0C
#define NRF_RX_ADDR_P3_REG			0x0D
#define NRF_RX_ADDR_P4_REG			0x0E
#define NRF_RX_ADDR_P5_REG			0x0F
#define NRF_TX_ADDR_REG				0x10
#define NRF_RX_PW_P0_REG			0x11
#define NRF_RX_PW_P1_REG			0x12
#define NRF_RX_PW_P2_REG			0x13
#define NRF_RX_PW_P3_REG			0x14
#define NRF_RX_PW_P4_REG			0x15
#define NRF_RX_PW_P5_REG			0x16
#define NRF_FIFO_STATUS_REG			0x17
#define NRF_DYNPD_REG				0x1C
#define NRF_DYNPD_DPL_P0			(1u<<0)
#define NRF_DYNPD_DPL_P1			(1u<<1)
#define NRF_DYNPD_DPL_P2			(1u<<2)
#define NRF_DYNPD_DPL_P3			(1u<<3)
#define NRF_DYNPD_DPL_P4			(1u<<4)
#define NRF_DYNPD_DPL_P5			(1u<<5)
#define NRF_FEATURE_REG				0x1D
#define NRF_FEATURE_EN_DPL			(1u<<2)



#endif /* NRF24L01_H_ */