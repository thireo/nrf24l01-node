/*
 * adc.h
 *
 * Created: 13-04-2019 22:47:00
 *  Author: Andreas
 */ 
#ifndef VBAT_H_
#define VBAT_H_
#include "main.h"
#include "asf.h"
#include "status_codes.h"

void vbat_init();
uint16_t vbat_read();

#endif