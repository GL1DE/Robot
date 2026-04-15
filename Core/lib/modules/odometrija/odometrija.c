/*
 * odometrija.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#include "odometrija.h"
#include "../lib/peripheries/encoder/encoder.h"

#include <math.h>

float
C_INC2RAD = 0.00076699039, // rad/inc
radius_tocka = 0.0695635 * 0.5, // [m] prethothdno najbolje
rastojanje_tockova = 0.2847761; // [m] prethodno najbolje

float dt = 0.002;

int32_t
enc_r,
enc_l;

volatile float
x = 0,
y = 0,
theta = 0,
v = 0,
w = 0,
v_r = 0,
v_l = 0;


void odom_update()
{
	enc_r = enc1_get_delta_inc();
	enc_l = enc2_get_delta_inc();

	v_r = enc_r * C_INC2RAD * radius_tocka / dt;
	v_l = enc_l * C_INC2RAD * radius_tocka / dt;

	v = (v_r + v_l) * 0.5;
	w = (v_r - v_l) / rastojanje_tockova;

	theta += w * dt;
	theta = normalize_rad_angle(theta);

	x += v * dt * cosf(theta + w * dt * 0.5);
	y += v * dt * sinf(theta + w * dt * 0.5);

}

float normalize_rad_angle(float angle)
{
	if (angle > M_PI)
	{
		return angle - 2 * M_PI;
	}
	if (angle < -M_PI)
	{
		return angle + 2 * M_PI;
	}

	return angle;
}
