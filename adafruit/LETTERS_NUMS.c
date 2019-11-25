/*
 * LETTERS&NUMS.c
 *
 *  Created on: Nov 15, 2019
 *      Author: alvar
 */
#include "LETTERS_NUMS.h"

static const uint8_t LETTER_A[] = { 0x00, 0x3F, 0x7F, 0x48, 0x48, 0x7F, 0x3F, 0x00 };
static const uint8_t LETTER_B[] = { 0x00, 0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00 };
static const uint8_t LETTER_C[] = { 0x00, 0x3E, 0x7F, 0x41, 0x41, 0x63, 0x22, 0x00 };
static const uint8_t LETTER_D[] = { 0x00, 0x7F, 0x7F, 0x41, 0x41, 0x7F, 0x3E, 0x00 };
static const uint8_t LETTER_E[] = { 0x00, 0x7F, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 };

static const uint8_t LETTER_F[] = { 0x00, 0x7F, 0x7F, 0x48, 0x48, 0x40, 0x40, 0x00 };
static const uint8_t LETTER_G[] = { 0x00, 0x3E, 0x7F, 0x41, 0x47, 0x67, 0x26, 0x00 };
static const uint8_t LETTER_H[] = { 0x00, 0x7F, 0x7F, 0x08, 0x08, 0x7F, 0x7F, 0x00 };
static const uint8_t LETTER_I[] = { 0x00, 0x00, 0x41, 0x7F, 0x7F, 0x41, 0x00, 0x00 };
static const uint8_t LETTER_J[] = { 0x00, 0x06, 0x07, 0x41, 0x7F, 0x7E, 0x40, 0x00 };
static const uint8_t LETTER_K[] = { 0x00, 0x7F, 0x7F, 0x1C, 0x36, 0x63, 0x41, 0x00 };
static const uint8_t LETTER_L[] = { 0x00, 0x7F, 0x7F, 0x01, 0x01, 0x01, 0x01, 0x00 };
static const uint8_t LETTER_M[] = { 0x00, 0x7F, 0x7F, 0x30, 0x18, 0x30, 0x7F, 0x7F };
static const uint8_t LETTER_N[] = { 0x00, 0x7F, 0x7F, 0x30, 0x18, 0x0C, 0x7F, 0x7F };
static const uint8_t LETTER_O[] = { 0x00, 0x3E, 0x7F, 0x41, 0x41, 0x7F, 0x3E, 0x00 };
static const uint8_t LETTER_P[] = { 0x00, 0x7F, 0x7F, 0x44, 0x44, 0x7C, 0x38, 0x00 };
static const uint8_t LETTER_Q[] = { 0x00, 0x3C, 0x7E, 0x42, 0x46, 0x7F, 0x3D, 0x00 };
static const uint8_t LETTER_R[] = { 0x00, 0x7F, 0x7F, 0x4C, 0x4E, 0x7B, 0x31, 0x00 };
static const uint8_t LETTER_S[] = { 0x00, 0x32, 0x7B, 0x49, 0x49, 0x6F, 0x26, 0x00 };
static const uint8_t LETTER_T[] = { 0x00, 0x60, 0x40, 0x7F, 0x7F, 0x40, 0x60, 0x00 };
static const uint8_t LETTER_U[] = { 0x00, 0x7E, 0x7F, 0x01, 0x01, 0x7F, 0x7F, 0x00 };
static const uint8_t LETTER_V[] = { 0x00, 0x7C, 0x7E, 0x03, 0x03, 0x7E, 0x7C, 0x00 };
static const uint8_t LETTER_W[] = { 0x00, 0x7F, 0x7F, 0x06, 0x0C, 0x06, 0x7F, 0x7F };
static const uint8_t LETTER_X[] = { 0x00, 0x63, 0x77, 0x1C, 0x0C, 0x77, 0x63, 0x00 };
static const uint8_t LETTER_Y[] = { 0x00, 0x70, 0x78, 0x0F, 0x0F, 0x78, 0x70, 0x00 };
static const uint8_t LETTER_Z[] = { 0x00, 0x43, 0x47, 0x4D, 0x59, 0x71, 0x61, 0x00 };

static const uint8_t NUM_0[] = { 0x00, 0x7C, 0x44, 0x7C };
static const uint8_t NUM_1[] = { 0x00, 0x24, 0x7C, 0x04 };
static const uint8_t NUM_2[] = { 0x00, 0x5C, 0x54, 0x74 };
static const uint8_t NUM_3[] = { 0x00, 0x54, 0x54, 0x7C };
static const uint8_t NUM_4[] = { 0x00, 0x70, 0x10, 0x7C };
static const uint8_t NUM_5[] = { 0x00, 0x74, 0x54, 0x5C };
static const uint8_t NUM_6[] = { 0x00, 0x7C, 0x54, 0x5C };
static const uint8_t NUM_7[] = { 0x00, 0x40, 0x40, 0x7C };
static const uint8_t NUM_8[] = { 0x00, 0x7C, 0x54, 0x7C };
static const uint8_t NUM_9[] = { 0x00, 0x74, 0x54, 0x7C };

let_num_t LET_NUM_ascii_2_mat(g_ascii_table_t ascii)
{
	let_num_t ret_val;
	for(int a = 0; a < LETTER_SIZE; a++) {
		ret_val.letter[a] = 0x00;
	}
	if (ascii >= 'a' && ascii <= 'z') {
		ascii -= 32;
	}
	if (((ascii >= CERO) && (ascii <= NINE)) || ((ascii >= LET_A) && (ascii <= LET_Z))) {
		switch ((uint8_t) ascii)
		{
			case LET_A:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_A[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_B:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_B[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;

			break;
			case LET_C:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_C[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_D:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_D[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_E:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_E[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_F:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_F[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_G:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_G[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_H:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_H[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_I:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_I[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_J:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_J[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_K:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_K[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_L:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_L[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_M:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_M[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_N:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_N[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_O:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_O[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_P:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_P[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_Q:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_Q[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_R:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_R[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_S:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_S[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_T:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_T[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_U:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_U[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_V:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_V[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_W:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_W[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_X:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_X[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_Y:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_Y[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case LET_Z:
				for(int a = 0; a < LETTER_SIZE; a++)
					ret_val.letter[a] = LETTER_Z[a];
				ret_val.size = LETTER_SIZE;
				return ret_val;
			break;
			case CERO:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_0[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case ONE:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_1[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case TWO:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_2[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case THREE:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_3[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case FOUR:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_4[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case FIVE:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_5[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case SIX:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_6[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case SEVEN:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_7[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case EIGHT:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_8[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			case NINE:
				for(int a = 0; a < NUM_SIZE; a++)
					ret_val.letter[a] = NUM_9[a];
				ret_val.size = NUM_SIZE;
				return ret_val;
			break;
			default:
				//for(int a = 0; a < NUM_SIZE; a++)
				ret_val.letter[0] = '\0';
				ret_val.size = 0;
				return ret_val;
			break;
		}

	}
	return ret_val;
}
