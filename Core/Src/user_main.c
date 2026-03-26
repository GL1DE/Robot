/*
 * user_main.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#include "stm32f4xx.h"
#include "user_main.h"
#include "../lib/peripheries/timer/timer.h"
#include "../lib/peripheries/encoder/encoder.h"
#include "../lib/modules/pogon/pogon.h"
#include "../lib/modules/pozicija/pozicija.h"
#include "../lib/modules/odometrija/odometrija.h"
#include "../lib/modules/ax12/ax12.h"

#include "tim.h"
#include "usart.h"
#include <string.h>
#include <math.h>
#include "stm32f4xx_hal_uart.h"

typedef enum {
	POSA, POSB, POSC, POSD, POSE, POSF, POSG, POSH, FINISH,
} FSM_States_TypeDef;

typedef enum {
	AX_90DEG = 60,
	AX_180DEG = 150,
}AX_States_TypeDef;

FSM_States_TypeDef trenutni_korak = POSA;

void user_main()
{
	// Enable encoder interface on tim2 & tim5
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
	encoders_init();

	// Enable PWM on Motor1 & Motor2
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

	// Pokreni tajmer
	HAL_TIM_Base_Start_IT(&htim1);

	current_motion_state = IDLE;
//	ax_goal_position(1, AX_180DEG);

	flg_reverse = 0;

	while (1) {

			switch (trenutni_korak)
			{
			case(POSA):
			{
				robot_set_pose_ref(0.4, 0.4);
				if (current_motion_state == GOAL_REACHED)
				{
//					ax_goal_position(1, AX_90DEG);
//					trenutni_korak = POSB;
//					flg_reverse = 1;
//					current_motion_state = IDLE;
				}
				break;
			}
			case(POSB):
			{
				robot_set_pose_ref(0.4, 0.4);
				if (current_motion_state == GOAL_REACHED)
				{
					trenutni_korak = POSC;
					ax_goal_position(1, AX_180DEG);
					HAL_Delay(500);
					current_motion_state = IDLE;
				}
				break;
			}
			case(POSC):
				robot_set_pose_ref(0.4, 0);
				if(current_motion_state == GOAL_REACHED)
				{
					trenutni_korak = POSD;
					current_motion_state = IDLE;
				}
			break;

			case(POSD):
				robot_set_pose_ref(0, 0);
				if(current_motion_state == GOAL_REACHED)
					trenutni_korak = FINISH;
			break;

			case(FINISH):
				ax_goal_position(1, AX_90DEG+45);
			break;
			}
		}
}
