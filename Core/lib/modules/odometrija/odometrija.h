/*
 * odometrija.h
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#ifndef LIB_MODULES_ODOMETRIJA_ODOMETRIJA_H_
#define LIB_MODULES_ODOMETRIJA_ODOMETRIJA_H_

#include "stm32f4xx.h"

extern volatile float
x,
y,
theta,
v,
w,
v_r,
v_l;

extern float dt;

void odom_update();

float normalize_rad_angle(float angle);

#endif /* LIB_MODULES_ODOMETRIJA_ODOMETRIJA_H_ */
