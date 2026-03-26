/*
 * timer.c
 *
 *  Created on: Mar 19, 2026
 *      Author: Lenovo
 */

#include "timer.h"
#include "tim.h"
#include "../lib/modules/odometrija/odometrija.h"
#include "../lib/modules/pogon/pogon.h"
#include "../lib/modules/pozicija/pozicija.h"

volatile uint32_t
sys_ms = 0,
timeout_ms = 0;

volatile timeout_flgs_typedef timeout_flgs = {0};

void set_timeout_ms (uint32_t ms)
{
	if (timeout_flgs.timeout_start)
		return;

	timeout_flgs.timeout_start = 1;
	timeout_flgs.timeout_end = 0;
	timeout_ms = ms;
}

void delay_ms(uint32_t ms)
{
	uint32_t tmp = sys_ms;
	while ((sys_ms - tmp) < ms);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		sys_ms++;

		if((sys_ms % 2) == 0)
			odom_update();

		if((sys_ms % 2) == 0)
			motor_control_loop();

		if((sys_ms % 10) == 0)
			position_control_loop();


		if (timeout_flgs.timeout_start)
		{
			timeout_ms--;
			if (timeout_ms == 0)
			{
				timeout_flgs.timeout_end = 1;
				timeout_flgs.timeout_start = 0;
			}
		}


	}
}
