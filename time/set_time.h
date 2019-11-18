/*
 * set_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef TIME_SET_TIME_H_
#define TIME_SET_TIME_H_

#include "MK64F12.h"
#include "../fifo.h"
#include "../drivers/UART.h"

#define TIME_FIFO_SIZE 8

typedef enum{
	TIME_TERMINAL0,TIME_TERMINAL1
}time_profil_t;


/////////////////////////////////////////start display info///////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tset Time\r" };
static int8_t chat_menu_pos[] = { "\033[12:10H" };
static int8_t chat[] = { "Write time in \"hh/mm/ss\" format\r\n" };



void display(time_profil_t terminal);



#endif /* TIME_SET_TIME_H_ */
