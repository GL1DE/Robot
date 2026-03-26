/*
 * ax12.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Lenovo
 */

#include "ax12.h"

void ax_goal_position(uint8_t id, float angle)
{
	uint16_t angle_inc = (uint16_t)(angle * 1023 / 300.0);

	uint8_t params[] = {
			AX_GOAL_POSITION,
			(uint8_t)angle_inc & 0xFF,
			(uint8_t)((angle_inc >> 8) & 0xFF),
	};
	ax_send_msg(id, AX_WRITE, params, 3);
}

void ax_led_state(uint8_t id, uint8_t state)
{
	if (state > 0)
		state = 1;

	uint8_t led_state[] = { 25, state};
	ax_send_msg(id, AX_WRITE, led_state, 2);
}

void ax_send_msg(uint8_t id, uint8_t instruction, uint8_t* params, uint8_t n_params){

	uint8_t len = n_params + 2;

	uint16_t tmp = id + instruction + len;

	for (int i = 0; i < n_params; i ++)
		tmp += params[i];

	uint8_t chksum = ~(tmp & 0xFF);

	ax_send_byte(0xFF);
	ax_send_byte(0xFF);
	ax_send_byte(id);
	ax_send_byte(len);
	ax_send_byte(instruction);
	ax_send_byte_array(params, n_params);
	ax_send_byte(chksum);
}

void ax_send_byte(uint8_t data)
{
	while((USART1->SR & (0b1 << 6)) == 0);
	USART1->DR = data;
}

void ax_send_byte_array(uint8_t* data, uint8_t n_data)
{
	for(int i = 0; i < n_data; i++)
		ax_send_byte(data[i]);
}
