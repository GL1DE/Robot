/*
 * timer.h
 *
 *  Created on: Mar 19, 2026
 *      Author: Lenovo
 */

#ifndef LIB_TIMER_TIMER_H_
#define LIB_TIMER_TIMER_H_

#include "stm32f4xx.h"

typedef struct {
	uint8_t timeout_start : 1;
	uint8_t timeout_end : 1;
}timeout_flgs_typedef;

volatile extern timeout_flgs_typedef timeout_flgs;

void set_timeout_ms(uint32_t ms);
void delay_ms(uint32_t ms);

#endif /* LIB_TIMER_TIMER_H_ */
