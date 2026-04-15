/*
 * pozicija.h
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#ifndef LIB_MODULES_POZICIJA_POZICIJA_H_
#define LIB_MODULES_POZICIJA_POZICIJA_H_

typedef enum
{
	IDLE,
	ROTATE_TO_GOAL,
	TRANSLATE_TO_GOAL,
	ROTATE_TO_THETA,
	GOAL_REACHED
}MotionState_t;

extern volatile MotionState_t current_motion_state;
extern int flg_reverse;

extern volatile float
x_ref,
y_ref,
theta_ref;

void robot_set_pose_ref(float x, float y, float theta);
void position_control_loop();

#endif /* LIB_MODULES_POZICIJA_POZICIJA_H_ */
