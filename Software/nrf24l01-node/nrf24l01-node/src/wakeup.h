/*
 * extint.h
 *
 * Created: 14-04-2019 20:05:27
 *  Author: Andreas
 */ 


#ifndef WAKEUP_H_
#define WAKEUP_H_


#include "asf.h"
#include "main.h"
#include "uart.h"

void rotenc_init(void);
void configure_extint_channel_a(void);
void configure_extint_callbacks_A(void);
void callback_a(void);
void configure_extint_channel_b(void);
void configure_extint_callbacks_B(void);
void callback_b(void);

#endif /* EXTINT_H_ */