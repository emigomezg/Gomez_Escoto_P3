/*
 * screen.c
 *
 *  Created on: Nov 17, 2019
 *      Author: alvar
 */

#include "screen.h"
static array_to_disp_t array_to_disp;

void screen_clear_data(void)
{
	for(int a = 0; a < MAX_DATA_SIZE; a++) {
		if (a < SCREEN_H_SIZE)
			array_to_disp.in_display[a] = '\0';
		array_to_disp.max_data[a] = '\0';
	}
	array_to_disp.index = 0;
	array_to_disp.num_elements = 0;
}
void screen_add_Item_end(uint8_t ascii)
{
	let_num_t let1;

	if (array_to_disp.num_elements < (MAX_DATA_SIZE-4)) {
		let1 = LET_NUM_ascii_2_mat(ascii);
		uint8_t temp_index = array_to_disp.index;
		uint8_t b = 0;
		for(uint16_t a = array_to_disp.index; a < (let1.size + temp_index); a++, array_to_disp.index++, b++) {
			array_to_disp.max_data[a] = let1.letter[b];
		}
		array_to_disp.num_elements += let1.size;
	}
}
void screen_send_array_2mat(void)
{
	uint8_t b = 0;
	for(uint8_t a = 0; a < 16; a += 2, b++) {
		array_to_disp.in_display[a] = array_to_disp.max_data[b];
		array_to_disp.in_display[a + 1] = array_to_disp.max_data[b + 8];

	}


	HT16k33_sendMat(&array_to_disp.in_display[0]);

}
void screen_slideR(void)
{
	uint8_t index = array_to_disp.index - 1;
	uint8_t temp = array_to_disp.max_data[0];
	for(int16_t a = 1; a <= index; a++) {
		array_to_disp.max_data[a-1] = array_to_disp.max_data[a];
	}
	array_to_disp.max_data[index] = temp;
}
