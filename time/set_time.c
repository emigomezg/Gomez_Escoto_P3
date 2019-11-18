/*
 * set_time.c
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#include "set_time.h"
/////////////////////////////////////////start display info///////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tset Time\r" };
static int8_t chat_menu_pos[] = { "\033[12:10H" };
static int8_t chat[] = { "Write time in \"hh/mm/ss\" format\r\n" };
static int8_t return_status_pos = { "\033[13:10H" };
static int8_t chat[] = { "time change was: " };
static int8_t unsucc[] = { "Unsuccessful\n\r" };
static int8_t succ[] = { "successful\n\r" };

fifos_t time;

void display(time_profil_t terminal)
{
	if (FIFO_init(&time, TIME_FIFO_SIZE) == FIFO_SUCCESS) {

		switch ((uint8_t) ter)
		{
			case TIME_TERMINAL0:
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &title_pos[0]);
				UART_put_string(UART_0, &title[0]);
				UART_put_string(UART_0, &chat_menu_pos[0]);
				UART_put_string(UART_0, &chat[0]);
			break;
			case TIME_TERMINAL1:
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &title_pos[0]);
				UART_put_string(UART_1, &title[0]);
				UART_put_string(UART_1, &chat_menu_pos[0]);
				UART_put_string(UART_1, &chat[0]);
			break;

		}
	}

}
