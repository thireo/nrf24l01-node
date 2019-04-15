/*
 * extint.c
 *
 * Created: 14-04-2019 20:02:48
 *  Author: Andreas
 */ 

#include "wakeup.h"

void rotenc_init(void)
{
	configure_extint_channel_a();
	//configure_extint_channel_b();
	configure_extint_callbacks_A();
	//configure_extint_callbacks_B();
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;
	port_pin_set_config(SIG_DONE_PIN, &config_port_pin);
	port_pin_set_output_level(SIG_DONE_PIN,0);
	delay_ms(10);
	port_pin_set_output_level(SIG_DONE_PIN,1);
	delay_ms(10);
	port_pin_set_output_level(SIG_DONE_PIN,0);	
	system_interrupt_enable_global();
}


void configure_extint_channel_a(void)
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin           = SIG_WAKE;
	config_extint_chan.gpio_pin_mux       = SIG_WAKE_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_DOWN;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;
	config_extint_chan.filter_input_signal = false;
	config_extint_chan.wake_if_sleeping = true;
	extint_chan_set_config(14, &config_extint_chan);
}

void configure_extint_callbacks_A(void)
{
	extint_register_callback(callback_a,14,	EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(14,	EXTINT_CALLBACK_TYPE_DETECT);
}

void callback_a(void)
{
	uart_write("WAKEUP\r\n");
	port_pin_set_output_level(SIG_DONE_PIN,1);
	delay_ms(10);
	port_pin_set_output_level(SIG_DONE_PIN,0);	
	woke = true;
}

void configure_extint_channel_b(void)
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin           = SIG_DONE_PIN;
	config_extint_chan.gpio_pin_mux       = SIG_DONE_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_DOWN;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	config_extint_chan.filter_input_signal = false;
	extint_chan_set_config(15, &config_extint_chan);
}

void configure_extint_callbacks_B(void)
{
	extint_register_callback(callback_b,15,	EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(15,	EXTINT_CALLBACK_TYPE_DETECT);
}

void callback_b(void)
{
	uart_write("booo2b");
}