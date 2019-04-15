/*
 * uart.h
 *
 * Created: 12-04-2019 23:31:00
 *  Author: Andreas
 */ 

#ifndef UART_H_
#define UART_H_

#include "main.h"
#include "asf.h"
#include "status_codes.h"

#define UART_MODULE		SERCOM5
#define UART_BAUDRATE	115200
#define UART_RX_PIN		5  // 
#define UART_TX_PIN		2//PORT_PB02 // PB02 - TX
#define UART_TIMEOUT	0xFFFF

#define MAX_RX_BUFFER_LENGTH   5

void uart_clk_init(void);
void uart_pin_init(void);
void uart_init(void);
enum status_code uart_write(uint8_t *data);
enum status_code uart_read(uint8_t *read_data, uint8_t data_length);


#endif /* UART_H_ */