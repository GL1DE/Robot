/*
 * pogon.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#include "pogon.h"
#include "../lib/modules/odometrija/odometrija.h"
#include <math.h>

const float
v_max = 1, 		// m/s
motor_acc = 0.2; 	// m/s^2 --> zeljeno ubrzanje

// PID regulator
volatile float
motor_r_output = 0.0,
motor_l_output = 0.0,
motor_r_prev_error = 0.0,
motor_l_prev_error = 0.0;

const float
Kp_r = 10,
Ki_r = 0.4,
Kp_l = 10,
Ki_l = 0.4;

volatile float
vr_m = 0,
vl_m = 0;

volatile float
vr_ref = 0.0,
vl_ref = 0.0;

volatile float
vr_trapez = 0.0,
vl_trapez = 0.0;


void motor_set_ref_vel(float v, float w)
{
	vr_ref = v + w * 0.5 * MOTOR_WHEEL_SEPARATION;
	vl_ref = v - w * 0.5 * MOTOR_WHEEL_SEPARATION;

	if (vr_ref > v_max)
		vr_ref = v_max;
	else if (vr_ref < -v_max)
		vr_ref = -v_max;

	if (vl_ref > v_max)
		vl_ref = v_max;
	else if (vl_ref < -v_max)
		vl_ref = -v_max;
}



void motor_control_loop()
{
	uint32_t ir_sensor_c = (GPIOC->IDR & (0b1 << 13 | 0b1 << 14 | 0b1 << 15)); // IR senzori na c pinovima
	uint32_t ir_sensor_b = (GPIOB->IDR & (0b1 << 7)); // IR senzori na b pinovima
	if (ir_sensor_c || ir_sensor_b)
	{
		const float alpha = 0.99;
		float acc_step = motor_acc * dt;

		vr_trapez *= alpha;
		vl_trapez *= alpha;
		if (fabsf(vr_trapez) < acc_step || fabsf(vl_trapez) < acc_step)
		{
			vr_trapez = 0.0;
			vl_trapez = 0.0;
		}
	}
	else
	{
		/* RIGHT */
		// trapezni profil
		if (fabsf(vr_trapez) < fabsf(vr_ref))
		{
			float acc_step = motor_acc * dt;
			float difference = vr_ref - vr_trapez;

			if (fabsf(difference) > acc_step)
			{
				if (vr_ref > 0)
					vr_trapez += acc_step;
				else if (vr_ref < 0)
					vr_trapez -= acc_step;
			}
			else
			{
				vr_trapez = vr_ref;
			}
		}
		else
		{
			vr_trapez = vr_ref;
		}




		/* LEFT */
		// trapezni profil
		if (fabsf(vl_trapez) < fabsf(vl_ref))
		{
			float acc_step = motor_acc * dt;
			float difference = vl_ref - vl_trapez;

			if (fabsf(difference) > acc_step)
			{
				if (difference > 0)
					vl_trapez += acc_step;
				else if (difference < 0)
					vl_trapez -= acc_step;
			}
			else
			{
				vl_trapez = vl_ref;
			}
		}
		else
		{
			vl_trapez = vl_ref;
		}
	}

	// estimcija brzine pogonskog tocka [m/s]
	const float beta = 0.3;
	// y[k] = (1-beta) * y[k-1] + beta * x[k]
	vr_m = (1-beta) * vr_m + beta * (v + 0.5 * w * MOTOR_WHEEL_SEPARATION);
	vl_m = (1-beta) * vl_m + beta * (v - 0.5 * w * MOTOR_WHEEL_SEPARATION);

	float error_r = vr_trapez - vr_m;
	float error_l = vl_trapez - vl_m;

	motor_r_output += Kp_r * (error_r - motor_r_prev_error) + Ki_r * error_r;
	motor_l_output += Kp_l * (error_l - motor_l_prev_error) + Ki_l * error_l;

	motor_r_prev_error = error_r;
	motor_l_prev_error = error_l;

	// saturacija izlaza iz regulatora
	if (motor_r_output > MOTOR_VOLTAGE)
		motor_r_output = MOTOR_VOLTAGE;
	else if (motor_r_output < -MOTOR_VOLTAGE)
		motor_r_output = -MOTOR_VOLTAGE;

	if (motor_l_output > MOTOR_VOLTAGE)
		motor_l_output = MOTOR_VOLTAGE;
	else if (motor_l_output < -MOTOR_VOLTAGE)
		motor_l_output = -MOTOR_VOLTAGE;


	set_motor1_voltage(motor_r_output);
	set_motor2_voltage(motor_l_output);

}

void set_motor1_dir(const MotorDir_t dir)
{
	switch(dir)
	{
	case FORWARD:
		HAL_GPIO_WritePin(M1_Dir_GPIO_Port, M1_Dir_Pin, 0);
		break;

	case BACKWARD:
		HAL_GPIO_WritePin(M1_Dir_GPIO_Port, M1_Dir_Pin, 1);
		break;
	}
}

void set_motor2_dir(const MotorDir_t dir)
{
	switch(dir)
	{
	case FORWARD:
		HAL_GPIO_WritePin(M2_Dir_GPIO_Port, M2_Dir_Pin, 0);
		break;

	case BACKWARD:
		HAL_GPIO_WritePin(M2_Dir_GPIO_Port, M2_Dir_Pin, 1);
		break;
	}
}

void set_motor1_voltage(float voltage)
{
	// saturacija napona
	if (voltage > MOTOR_VOLTAGE)
		voltage = MOTOR_VOLTAGE;
	else if (voltage < -MOTOR_VOLTAGE)
		voltage = -MOTOR_VOLTAGE;


	if (voltage > 0)
	{
		set_motor1_dir(FORWARD);
		TIM3->CCR2 = (uint32_t)(voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
	else if (voltage < 0)
	{
		set_motor1_dir(BACKWARD);
		TIM3->CCR2 = (uint32_t)(-voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}

}

void set_motor2_voltage(float voltage)
{
	// saturacija napona
	if (voltage > MOTOR_VOLTAGE)
		voltage = MOTOR_VOLTAGE;
	else if (voltage < -MOTOR_VOLTAGE)
		voltage = -MOTOR_VOLTAGE;


	if (voltage > 0)
	{
		set_motor2_dir(FORWARD);
		TIM4->CCR1 = (uint32_t)(voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
	else if (voltage < 0)
	{
		set_motor2_dir(BACKWARD);
		TIM4->CCR1 = (uint32_t)(-voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
}
