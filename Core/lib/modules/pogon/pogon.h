/*
 * pogon.h
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#ifndef LIB_MODULES_POGON_POGON_H_
#define LIB_MODULES_POGON_POGON_H_

#include "stm32f4xx.h"
#include "main.h"

#define MOTOR_VOLTAGE 		12.0 // [V]
#define MOTOR_ARR			2099.0
#define MOTOR_WHEEL_SEPARATION 0.2847761 * 0.5

typedef enum
{
	FORWARD,
	BACKWARD
}MotorDir_t;

extern volatile float
vr_m,
vl_m;

extern volatile float
error_l,
error_r,
motor_l_prev_error,
motor_r_prev_error;

extern const float
v_max;

extern volatile float
vr_ref,
vl_ref;

extern volatile float
vr_trapez,
vl_trapez;

void set_motor1_dir(const MotorDir_t dir);
void set_motor2_dir(const MotorDir_t dir);

void set_motor1_voltage(float voltage);
void set_motor2_voltage(float voltage);

void motor_set_ref_vel(float v, float w);
void motor_control_loop();

#endif /* LIB_MODULES_POGON_POGON_H_ */
