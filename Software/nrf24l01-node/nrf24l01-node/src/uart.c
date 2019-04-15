/*
 * uart.c
 *
 * Created: 12-04-2019 23:31:00
 *  Author: Andreas
 */ 

#include "uart.h"


void uart_clk_init(void)
{
	// Start the Software Reset and wait for it to finish
	UART_MODULE->USART.CTRLA.bit.SWRST = 1 ;
	while ( UART_MODULE->USART.CTRLA.bit.SWRST || UART_MODULE->USART.SYNCBUSY.bit.SWRST );
	
	// Turn on peripheral clock for SERCOM being used
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM5;

	//Setting clock
	GCLK->CLKCTRL.reg =
	GCLK_CLKCTRL_ID( GCM_SERCOM5_CORE )	|	// connected  SERCOMx to
	GCLK_CLKCTRL_GEN_GCLK0		|	// generic Clock Generator 0
	GCLK_CLKCTRL_CLKEN;			// and enable it

	while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ); // Wait for synchronization
}

void uart_pin_init(void)
{
	//PORT->Group[PORTGROUP_A].DIRCLR.reg = PORT_PA05;	// RX as input
	
	PORT->Group[PORTGROUP_B].DIRSET.reg = PORT_PB02;	// TX as output
	PORT->Group[PORTGROUP_B].OUTSET.reg = PORT_PB02;	// TX idle state is high

	// set port multiplexer for peripheral TX
	// =======================================
	uint32_t temp = (PORT->Group[PORTGROUP_B].PMUX[UART_TX_PIN>>1].reg) & PORT_PMUX_PMUXO( PORT_PMUX_PMUXO_D_Val );
	PORT->Group[PORTGROUP_B].PMUX[UART_TX_PIN>>1].reg = temp | PORT_PMUX_PMUXE( PORT_PMUX_PMUXE_D_Val );
	
	PORT->Group[PORTGROUP_B].PINCFG[UART_TX_PIN].reg = PORT_PINCFG_PMUXEN ; // Enable port mux
	temp = (PORT->Group[PORTGROUP_B].PMUX[UART_RX_PIN>>1].reg) & PORT_PMUX_PMUXO( PORT_PMUX_PMUXO_D_Val );
	PORT->Group[PORTGROUP_B].PMUX[UART_RX_PIN>>1].reg = temp | PORT_PMUX_PMUXE( PORT_PMUX_PMUXE_D_Val );
	PORT->Group[PORTGROUP_B].PINCFG[UART_RX_PIN].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN; // Enable port mux
}

void uart_init(void)
{
	uart_clk_init();
	uart_pin_init();
	
	UART_MODULE->USART.CTRLA.reg =
	SERCOM_USART_CTRLA_DORD						|	// Lobd_FIRST
	SERCOM_USART_CTRLA_TXPO(0)					|	// TX on Pad2
	SERCOM_USART_CTRLA_RXPO(2)					|	// RX on Pad0
	SERCOM_USART_CTRLA_SAMPR(0)					|	// 16 times oversampling
	SERCOM_USART_CTRLA_RUNSTDBY					|	// Run in standby mode
	SERCOM_USART_CTRLA_MODE_USART_INT_CLK;			// Use internal clock
	
	// Asynchronous arithmetic mode
	// 65535 * ( 1 - sampleRateValue * baudrate / SystemCoreClock);
	// 65535 - 65535 * (sampleRateValue * baudrate / SystemCoreClock));
	UART_MODULE->USART.BAUD.reg = 65535.0f * ( 1.0f - (16.0 * (float)(UART_BAUDRATE)) / (float)(SYSTEM_CLK));
	//BLE_UART_SERCOM->USART.BAUD.bit.BAUD = 9600;
	
	UART_MODULE->USART.CTRLB.reg =
	SERCOM_USART_CTRLB_CHSIZE(0)	|	// 8 bit character size
	SERCOM_USART_CTRLB_TXEN;			//|	// Enable Transmit
	//SERCOM_USART_CTRLB_RXEN;			// Enable Receive

	// Get Synced
	while (UART_MODULE->USART.SYNCBUSY.bit.CTRLB);

	//Set the Interrupt to use
	UART_MODULE->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC;	// Interrupt on received complete
	
	// Enable interrupts
	NVIC_EnableIRQ(SERCOM5_IRQn);
	NVIC_SetPriority(SERCOM5_IRQn,1);
	
	// enable the peripheral block
	UART_MODULE->USART.CTRLA.bit.ENABLE = 0x1u;
	
	// Wait for sercom to enable
	while(UART_MODULE->USART.SYNCBUSY.bit.ENABLE);
}

enum status_code uart_write(uint8_t *data)
{
	uint32_t i = 0;
	while(data[i] != '\0')
	{
		
		if(UART_MODULE->USART.INTFLAG.bit.DRE == 1)
		{
			UART_MODULE->USART.DATA.reg = (uint16_t)data[i++];
		}
	}
	return STATUS_OK;
}

enum status_code uart_read(uint8_t *read_data, uint8_t data_length)
{
	uint8_t rx_pos = 0;
	
	enum status_code return_status = STATUS_OK;
	
	while(data_length--)
	{
		//Waiting for data...
		for (uint32_t i=0;i<=UART_TIMEOUT;i++)
		{
			if (UART_MODULE->USART.INTFLAG.bit.RXC)
			{
				break;
			}
			else if (i==UART_TIMEOUT)
			{
				return_status = STATUS_ERR_TIMEOUT;
			}
		}
		//Reading data into read_data
		read_data[rx_pos++] = (uint8_t) UART_MODULE->USART.DATA.reg;
	}
	return return_status;
}

