/*
 * odometrija.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#include "odometrija.h"
#include "../lib/peripheries/encoder/encoder.h"

#include <math.h>

const float
C_INC2RAD = 0.00076699039, // rad/inc
radius_tocka = 0.0775 / 2, // [m] PROMENITI VREDNOST NA PRECNIK NASEG ENKODERSKOG TOCKA
rastojanje_tockova = 0.2605; // [m] PROMENITI VREDNOST NA RASTOJANJE IZMEDJU ENKODERSKIH TOCKOVA

float dt = 0.002;

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
	int32_t enc_l = enc1_get_delta_inc();
	int32_t enc_r = enc2_get_delta_inc();

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
