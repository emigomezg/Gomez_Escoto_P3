/*
 * screen.h
 *
 *  Created on: Nov 17, 2019
 *      Author: alvar
 */

#ifndef ADAFRUIT_SCREEN_H_
#define ADAFRUIT_SCREEN_H_
#include "adafruit/ht16k33.h"
#include "adafruit/LETTERS_NUMS.h"

//#define MAX_DATA_SIZE 248
#define MAX_DATA_SIZE 25
#define SCREEN_H_SIZE 16

typedef struct
{
	 uint8_t in_display[SCREEN_H_SIZE];
	 uint8_t max_data[MAX_DATA_SIZE];
	 uint16_t index;
	 uint32_t num_elements;
//uint8_t write_available; solo una terminal a a la vez puede editar in_display

}array_to_disp_t;




void screen_clear_data(void);
void screen_slideR(void);
void screen_send_array_2mat(void);
void screen_add_Item_end(uint8_t ascii);



#endif /* ADAFRUIT_SCREEN_H_ */
