/*
 * encoder.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#include "encoder.h"

void encoders_init()
{
	TIM2->CNT = ENC_START_INC;
	TIM5->CNT = ENC_START_INC;
}

int32_t enc1_get_delta_inc()
{
	int32_t tmp = (TIM2->CNT - ENC_START_INC);
	TIM2->CNT = ENC_START_INC;
	return tmp;
}

int32_t enc2_get_delta_inc()
{
	int32_t tmp = (TIM5->CNT - ENC_START_INC);
	TIM5->CNT = ENC_START_INC;
	return tmp;
}
