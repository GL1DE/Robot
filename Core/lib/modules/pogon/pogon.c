/*
 * pogon.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

//MOTOR WHEEL CIRCUMFRENCE = 0.040;
//MAX SPEED = 263 rot/min = 4.383333 rot/sec
//maxWheel speed = 2motorwheelcircumpferncepi * 4.383333 at 12v
// maxWheel speed = 0.2512 * 4.383333
// 12V = maxWheel speed = 1.1
// 1.1 / vref = 12 / voltaza
//voltaza = 12*vref/1.1

#include "pogon.h"
#include "../lib/modules/odometrija/odometrija.h"
#include <math.h>
#include "../lib/modules/pozicija/pozicija.h"

const float
v_max = 3, 		// m/s
motor_acc = 2.5; 	// m/s^2 --> zeljeno ubrzanje

// PID regulator
volatile float
motor_r_output = 0.0,
motor_l_output = 0.0,
motor_r_prev_error = 0.0,
motor_l_prev_error = 0.0;

float
Kp_r = 0.8,
Ki_r = 0.07,
Kp_l = 0.8,
Ki_l = 0.07065,
motor_acc_levi = 5,
motor_acc_desni = 5;


volatile float
vr_m = 0,
vl_m = 0;

volatile float
vr_ref = 0.0,
vl_ref = 0.0;

volatile float
vr_trapez = 0.0,
vl_trapez = 0.0;

volatile float
error_r,
error_l,
voltage_r_ref,
voltage_l_ref;


void motor_set_ref_vel(float v, float w)
{
	vr_ref = v + w * 0.5 * MOTOR_WHEEL_SEPARATION;
	vl_ref = v - w * 0.5 * MOTOR_WHEEL_SEPARATION;

	if (vr_ref > v_max)
		vr_ref = v_max;
	if (vr_ref < -v_max)
		vr_ref = -v_max;

	if (vl_ref > v_max)
		vl_ref = v_max;
	if (vl_ref < -v_max)
		vl_ref = -v_max;
}



void motor_control_loop()
{
	if(flg_reverse)
	{
		Kp_r = 0.88,
		Ki_r = 0.072,
		Kp_l = 0.8,
		Ki_l = 0.07,
		motor_acc_levi = 5,
		motor_acc_desni = 5;
	}

	uint32_t ir_sensor_PD = (GPIOC->IDR & (0b1 << 13)); // PREDNJI DESNI
	uint32_t ir_sensor_ZL = (GPIOB->IDR & (0b1 << 7)); //  ZADNJI LEVI
	uint32_t ir_sensor_ZD = (GPIOC->IDR & (0b1 << 2)); // ZADNJI DESNI
	uint32_t ir_sensor_PL = (GPIOC->IDR & (0b1 << 3));
	if (ir_sensor_PL)
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
			float acc_step = motor_acc_desni * dt;
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
			float acc_step = motor_acc_levi * dt;
			float difference = vl_ref - vl_trapez;

			if (fabsf(difference) > acc_step)
			{
				if (vl_ref > 0)
					vl_trapez += acc_step;
				else if (vl_ref < 0)
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
	const float beta = 0.8;
	// y[k] = (1-beta) * y[k-1] + beta * x[k]
	vr_m = (1-beta) * vr_m + beta * (v + 0.5 * w * MOTOR_WHEEL_SEPARATION);
	vl_m = (1-beta) * vl_m + beta * (v - 0.5 * w * MOTOR_WHEEL_SEPARATION);

	error_r = vr_trapez - vr_m;
	error_l = vl_trapez - vl_m;

	float ki_error_r = Ki_r * error_r;
	float ki_error_l = Ki_l * error_l;

	if(ki_error_r > 10)
		ki_error_r = 10;
	if(ki_error_r < -10)
		ki_error_r = -10;
	if(ki_error_l > 10)
		ki_error_l = 10;
	if(ki_error_l < -10)
		ki_error_l = -10;


	motor_r_output += Kp_r * (error_r - motor_r_prev_error) + ki_error_r; // brzina
	motor_l_output += Kp_l * (error_l - motor_l_prev_error) + ki_error_l; // brzina

	motor_r_prev_error = error_r;
	motor_l_prev_error = error_l;

	voltage_r_ref = 12 * motor_r_output / v_max;
	voltage_l_ref = 12 * motor_l_output / v_max;

	// saturacija izlaza iz regulatora
	if (motor_r_output > MOTOR_VOLTAGE)
		motor_r_output = MOTOR_VOLTAGE;
	if (motor_r_output < -MOTOR_VOLTAGE)
		motor_r_output = -MOTOR_VOLTAGE;

	if (motor_l_output > MOTOR_VOLTAGE)
		motor_l_output = MOTOR_VOLTAGE;
	if (motor_l_output < -MOTOR_VOLTAGE)
		motor_l_output = -MOTOR_VOLTAGE;

	if (voltage_r_ref > MOTOR_VOLTAGE)
		voltage_r_ref = MOTOR_VOLTAGE;
	if (voltage_r_ref < -MOTOR_VOLTAGE)
		voltage_r_ref = -MOTOR_VOLTAGE;

	if (voltage_l_ref > MOTOR_VOLTAGE)
		voltage_l_ref = MOTOR_VOLTAGE;
	if (voltage_l_ref < -MOTOR_VOLTAGE)
		voltage_l_ref = -MOTOR_VOLTAGE;


	set_motor1_voltage(motor_r_output); // desni
	set_motor2_voltage(motor_l_output); // levi



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
	if (voltage < -MOTOR_VOLTAGE)
		voltage = -MOTOR_VOLTAGE;


	if (voltage > 0)
	{
		set_motor1_dir(FORWARD);
		TIM3->CCR2 = (uint32_t)(voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
	if (voltage < 0)
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
	if (voltage < -MOTOR_VOLTAGE)
		voltage = -MOTOR_VOLTAGE;


	if (voltage > 0)
	{
		set_motor2_dir(FORWARD);
		TIM4->CCR1 = (uint32_t)(voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
	if (voltage < 0)
	{
		set_motor2_dir(BACKWARD);
		TIM4->CCR1 = (uint32_t)(-voltage * MOTOR_ARR/MOTOR_VOLTAGE);
	}
}
