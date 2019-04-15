/*
 * adc.c
 *
 * Created: 13-04-2019 22:47:00
 *  Author: Andreas
 */ 

#include "vbat.h"

void vbat_init()
{
	PORT->Group[PORTGROUP_A].DIRSET.reg = VBAT_ADC_EN_PORT;	
	PORT->Group[PORTGROUP_A].OUTCLR.reg = VBAT_ADC_EN_PORT;	
		
    REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_ADC;
    REG_PM_APBCMASK |= PM_APBCMASK_ADC; //Enable ADC Clock

    //Configure AN1
    PORT->Group[PORTGROUP_A].PINCFG[VBAT_ADC_PIN].bit.INEN = 0;
    PORT->Group[PORTGROUP_A].DIRCLR.reg = VBAT_ADC_PORT;
    PORT->Group[PORTGROUP_A].PINCFG[VBAT_ADC_PIN].bit.PULLEN = 0;
    PORT->Group[PORTGROUP_A].PINCFG[VBAT_ADC_PIN].bit.PMUXEN = 1;        //enable alternate function
    PORT->Group[PORTGROUP_A].PMUX[1].reg |= PORT_PMUX_PMUXO_B; //Enable alternate function, Analog Input
    REG_ADC_CTRLA |= ADC_CTRLA_ENABLE;
    while (ADC->STATUS.bit.SYNCBUSY)
    {
    }                                                                      //Wait for Enable
    REG_ADC_REFCTRL |= ADC_REFCTRL_REFSEL_INTVCC0;                           // | ADC_REFCTRL_REFCOMP;  //1V Reference
    REG_ADC_CTRLB |= ADC_CTRLB_PRESCALER_DIV4 | ADC_CTRLB_RESSEL_12BIT;    // | ADC_CTRLB_DIFFMODE;
    REG_ADC_AVGCTRL |= ADC_AVGCTRL_SAMPLENUM_32 | ADC_AVGCTRL_ADJRES(0x4); //Set Averaging to 32 Sample
    REG_ADC_EVCTRL |= ADC_EVCTRL_RESRDYEO;                                 //enable conversion complete
    REG_ADC_SAMPCTRL |= ADC_SAMPCTRL_RESETVALUE;
}
uint16_t vbat_read()
{
    REG_ADC_INPUTCTRL = ADC_INPUTCTRL_GAIN_1X | ADC_INPUTCTRL_INPUTSCAN(0x00) | ADC_INPUTCTRL_MUXNEG_GND | ADC_INPUTCTRL_MUXPOS_PIN7; //Configure ADC
    REG_ADC_SWTRIG |= ADC_SWTRIG_START;                                                                                                 //Start ADC
    while ((ADC->INTFLAG.bit.RESRDY == 0))
    {
    }
    uint16_t Result = REG_ADC_RESULT;

    return Result;
}