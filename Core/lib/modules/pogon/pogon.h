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
#define MOTOR_ARR			2099
#define MOTOR_WHEEL_SEPARATION 0.18-0.0302 //ZAMENITI ZA NASU VREDNOST 30.2

typedef enum
{
	FORWARD,
	BACKWARD
}MotorDir_t;

extern volatile float
vr_m,
vl_m;

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
