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
	Lift_Desni = 11,
	Lift_Levi = 4,
	Sisaljka_desna = 7,
	Sisaljka_leva = 5,
	Gripper_Hvat = 13,
	Gripper_lift = 6,
	Gripper_Rotate = 3,
	Termometar = 8,
}AX_ID_Typedef;

typedef enum {
	Lift_Desni_Gore = 5,
	Lift_Desni_Dole = 273,
	Lift_Levi_Gore = 150, //podesiti
	Lift_Levi_Dole = 180, // isto podesiti
	Sisaljka_Leva_levo = 195,
	Sisaljka_Leva_Desno = 12,
	Sisaljka_Desna_levo = 285,
	Sisaljka_Desna_Desno = 102,
	Termometar_Start = 200,
	Termometar_Spustena = 300, //Termometar obe vrednosti ce se menjati
	Gripper_ispravan = 240,
	Gripper_obrnut = 60,


}AX_States_TypeDef;

FSM_States_TypeDef trenutni_korak = POSA;

uint8_t
tmp = 0,
flg = 0;


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




//	pokupi_stek();

	flg_reverse = 1;
//	while(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4))
//		continue;

	while (1)
	{
//		flg = 1;


//		if(!cinc)
//			flg = 1;

		if(flg)
			tmp = 1;

//		while(flg)
//		{

			switch (trenutni_korak)
			{
			case(POSA):

//		set_motor1_voltage(12); // desni
//			set_motor2_voltage(12);
//			motor_set_ref_vel(-3/2, 0);

				robot_set_pose_ref(0, 0, M_PI_2*0.5);
//				if (current_motion_state == GOAL_REACHED)
//				{
//////					ax_goal_position(1, AX_90DEG);
////					trenutni_korak = POSB;
//////					flg_reverse = 1;
////					current_motion_state = IDLE;

				break;

			case(POSB):

//				robot_set_pose_ref(0.4, 0.4);
				if (current_motion_state == GOAL_REACHED)
				{
					trenutni_korak = POSC;
//					ax_goal_position(1, AX_180DEG);
//					HAL_Delay(500);
					current_motion_state = IDLE;
				}
				break;

			case(POSC):
//				robot_set_pose_ref(0.4, 0);
				if(current_motion_state == GOAL_REACHED)
				{
					trenutni_korak = POSD;
					current_motion_state = IDLE;
				}
			break;

			case(POSD):

//				robot_set_pose_ref(0, 0);
				if(current_motion_state == GOAL_REACHED)
					trenutni_korak = FINISH;
			break;
//
			case(FINISH):
//				ax_goal_position(1, AX_90DEG+45);
			break;

			}
//		}
	}
}

void pokupi_stek()
{
	ax_goal_position(4, 200);
	HAL_Delay(2000);
	ax_goal_position(4, 300);
	HAL_Delay(2000);
}
