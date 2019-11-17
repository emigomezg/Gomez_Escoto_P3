/*
 * LETTERS&NUMS.h
 *
 *  Created on: Nov 15, 2019
 *      Author: alvar
 */

#ifndef ADAFRUIT_LETTERS_NUMS_H_
#define ADAFRUIT_LETTERS_NUMS_H_
#include "MK64F12.h"
#define LETTER_SIZE 8
#define NUM_SIZE 4


typedef struct
{
	uint8_t letter[LETTER_SIZE];
	uint8_t number[NUM_SIZE];
	uint8_t size ;
} let_num_t;
typedef enum
{
	CERO = 48,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	LET_A = 65,
	LET_B,
	LET_C,
	LET_D,
	LET_E,
	LET_F,
	LET_G,
	LET_H,
	LET_I,
	LET_J,
	LET_K,
	LET_L,
	LET_M,
	LET_N,
	LET_O,
	LET_P,
	LET_Q,
	LET_R,
	LET_S,
	LET_T,
	LET_U,
	LET_V,
	LET_W,
	LET_X,
	LET_Y,
	LET_Z
}g_ascii_table_t;
let_num_t LET_NUM_ascii_2_mat(g_ascii_table_t ascii);

#endif /* ADAFRUIT_LETTERS_NUMS_H_ */
