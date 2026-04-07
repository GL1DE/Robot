/*
 * pozicija.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#include "pozicija.h"
#include "../lib/modules/odometrija/odometrija.h"
#include "../lib/modules/pogon/pogon.h"
#include <math.h>

int flg_reverse = 0;

const float // ove menjati
Kp_rot = 3.0,
Kp_trans = 4.0;

const float
eps_theta = 0.01745329251994329576923690768489*3, // 1 deg * x Probati smanjiti na pola stepena
eps_distance = 0.01*3; // 1cm * x Treba smanjiti u miliimetar

volatile MotionState_t current_motion_state = IDLE;
volatile float x_ref, y_ref, theta_ref;

volatile float
phi,
phi_error,
theta_error,
dx,
dy,
distance;

float v_ref = 0, w_ref = 0;

void robot_set_pose_ref(float x, float y)
{
	if (current_motion_state == IDLE)
	{
		x_ref = x;
		y_ref = y;

		current_motion_state = ROTATE_TO_GOAL;
	}
}

void position_control_loop()
{

	dx = x_ref - x;
	dy = y_ref - y;
	phi = normalize_rad_angle(atan2f(dy, dx));

	if(flg_reverse)
		phi_error = normalize_rad_angle(phi - theta + M_PI);
	else
		phi_error = normalize_rad_angle(phi - theta);

	distance = sqrtf(dx * dx + dy * dy);
	theta_error = normalize_rad_angle(theta_ref - theta);

	switch(current_motion_state)
	{
	case ROTATE_TO_GOAL:

			v_ref = 0.0;
			w_ref = Kp_rot * phi_error;

			if (fabsf(phi_error) < eps_theta && w == 0)
			{
				v_ref = 0.0;
				w_ref = 0.0;
				current_motion_state = TRANSLATE_TO_GOAL;
			}


		break;

	case TRANSLATE_TO_GOAL:
			if(!flg_reverse) // Ako nije podignut flag za rikverc idi unapred
				v_ref = Kp_trans * distance;
			else
				v_ref = -Kp_trans * distance; //Ako je podignut flag za rikverc onda stavi negativnu referentnu brzinu

			if (distance > 0.1) //Ako nije stigao na odrediste ispravi robota
				w_ref = Kp_rot * phi_error;

			if (fabsf(phi_error) > M_PI_2) //Ako je otisao predaleko vrati ga nazad
				v_ref = -v_ref;

			if (distance < eps_distance && v == 0)
			{
				v_ref = 0.0;
				w_ref = 0.0;
				current_motion_state = GOAL_REACHED;
			}

		break;

	case GOAL_REACHED:
		current_motion_state = GOAL_REACHED;
		flg_reverse = 0;
		break;
	}


	motor_set_ref_vel(v_ref, w_ref);
}

