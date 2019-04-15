/*
 * main.h
 *
 * Created: 19-12-2017 17:46:26
 *  Author: Andreas
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "asf.h"

#define SYSTEM_CLK 8000000


// defines for SAMD21
#define PORTGROUP_A		0
#define PORTGROUP_B		1

#define GPIO_SERCOM_ALT_D 3 //peripheral function D - SERCOM-ALT

#define GCM_SERCOM0_CORE          (0x14U)
#define GCM_SERCOM1_CORE          (0x15U)
#define GCM_SERCOM2_CORE          (0x16U)
#define GCM_SERCOM3_CORE          (0x17U)
#define GCM_SERCOM4_CORE          (0x18U)
#define GCM_SERCOM5_CORE          (0x19U)

#define SAMPLE_RATE_x16 0


#define SOFTWARE_VERSION 42


#define NRF_VCC_EN PIN_PA17
#define SIG_WAKE PIN_PA14
#define SIG_WAKE_MUX PINMUX_PA14A_EIC_EXTINT14
#define SIG_DONE_PIN PIN_PA15
#define SIG_DONE_MUX PINMUX_PA15A_EIC_EXTINT15
#define VBAT_ADC_PIN PIN_PA07
#define VBAT_ADC_PORT PORT_PA07
#define VBAT_ADC_EN_PIN PIN_PA06
#define VBAT_ADC_EN_PORT PORT_PA06

bool woke;

#endif /* MAIN_H_ */