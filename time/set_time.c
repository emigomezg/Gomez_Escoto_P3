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
static int8_t return_status_pos[] = { "\033[13:10H" };
static int8_t return_status[] = { "time change was:\t" };
static int8_t unsucc[] = { "Unsuccessful\n\r" };
static int8_t succ[] = { "successful\n\r" };
static int8_t press_any_key_pos[] = { "\033[15:10H" };
static int8_t press_any_key[] = { "press any key to continue....\n\r" };

uint8_t set_time0_exit_flag = FALSE;
uint8_t set_time1_exit_flag = FALSE;

fifos_t time0, time1;

void SET_TIME_display(time_profil_t terminal)
{

	switch ((uint8_t) terminal)
	{
		case TIME_TERMINAL1:
			if (FIFO_init(&time0, TIME_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &title_pos[0]);
				UART_put_string(UART_0, &title[0]);
				UART_put_string(UART_0, &chat_menu_pos[0]);
				UART_put_string(UART_0, &chat[0]);
				set_time0_exit_flag = FALSE;
			} else {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &return_status_pos[0]);

				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &unsucc[0]);
				set_time0_exit_flag = FALSE;
			}
		break;
		case TIME_TERMINAL2:
			if (FIFO_init(&time1, TIME_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &title_pos[0]);
				UART_put_string(UART_1, &title[0]);
				UART_put_string(UART_1, &chat_menu_pos[0]);
				UART_put_string(UART_1, &chat[0]);
				set_time1_exit_flag = FALSE;
			} else {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &return_status_pos[0]);

				UART_put_string(UART_1, &return_status[0]);
				UART_put_string(UART_1, &unsucc[0]);
				set_time1_exit_flag = FALSE;
			}
		break;

	}

}
void SET_TIME_uart0_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
	static volatile uint8_t err_flag = FALSE;
	uint8_t data_recived = UART_get_mailbox(UART_0);
	if (wait_flag == TRUE) {
		set_time0_exit_flag = TRUE;
		wait_flag = FALSE;
		err_flag = FALSE;
	}else	if (data_recived != '\e' && data_recived != '\r' && FIFO_getStatus(&time0) != FIFO_FULL) {
		FIFO_push(&time0, data_recived);
		UART_put_char(UART_0, data_recived);
	} else if (data_recived == '\r' && time0.index < (TIME_FIFO_SIZE - 1)) {
		UART_put_string(UART_0, &return_status_pos[0]);
		UART_put_string(UART_0, &return_status[0]);
		UART_put_string(UART_0, &unsucc[0]);
		wait_flag = TRUE;
		UART_put_string(UART_0, &press_any_key_pos[0]);
		UART_put_string(UART_0, &press_any_key[0]);
	} else if (data_recived == '\e') {
		set_time0_exit_flag = TRUE;
	} else if (time0.status == FIFO_FULL) {
		uint8_t hh[2];
		hh[0] = FIFO_POP(&time0);
		hh[1] = FIFO_POP(&time0);
		FIFO_POP(&time0);
		uint8_t mm[2];
		mm[0] = FIFO_POP(&time0);
		mm[1] = FIFO_POP(&time0);
		FIFO_POP(&time0);
		uint8_t ss[2];
		ss[0] = FIFO_POP(&time0);
		ss[1] = FIFO_POP(&time0);
		uint8_t hours = (hh[0] - '0') * 10 + (hh[1] - '0');
		uint8_t minutes = (mm[0] - '0') * 10 + (mm[1] - '0');
		uint8_t seconds = (ss[0] - '0') * 10 + (ss[1] - '0');
		if (hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60 && seconds >= 0 && seconds < 60) {
			err_flag = MCP7940M_set_seconds(seconds);
			MCP7940M_set_minutes(minutes);
			MCP7940M_set_hours(hours);
			if (err_flag) {
				UART_put_string(UART_0, &return_status_pos[0]);
				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &succ[0]);
				err_flag = FALSE;
			} else {
				UART_put_string(UART_0, &return_status_pos[0]);
				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &unsucc[0]);
				err_flag = FALSE;
			}
		} else {
			UART_put_string(UART_0, &return_status_pos[0]);
			UART_put_string(UART_0, &return_status[0]);
			UART_put_string(UART_0, &unsucc[0]);
		}
		wait_flag = TRUE;
		UART_put_string(UART_0, &press_any_key_pos[0]);
		UART_put_string(UART_0, &press_any_key[0]);

	}

}

void SET_TIME_uart1_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
		static volatile uint8_t err_flag = FALSE;
		uint8_t data_recived = UART_get_mailbox(UART_1);
		if (wait_flag == TRUE) {
			set_time1_exit_flag = TRUE;
			wait_flag = FALSE;
			err_flag = FALSE;
		}else if (data_recived != '\e' && data_recived != '\r' && FIFO_getStatus(&time0) != FIFO_FULL) {
			FIFO_push(&time1, data_recived);
			UART_put_char(UART_1, data_recived);
		} else if (data_recived == '\r' && time1.index < (TIME_FIFO_SIZE - 1)) {
			UART_put_string(UART_1, &return_status_pos[0]);
			UART_put_string(UART_1, &return_status[0]);
			UART_put_string(UART_1, &unsucc[0]);
			wait_flag = TRUE;
			UART_put_string(UART_1, &press_any_key_pos[0]);
			UART_put_string(UART_1, &press_any_key[0]);
		} else if (data_recived == '\e') {
			set_time1_exit_flag = TRUE;
		} else if (time1.status == FIFO_FULL) {
			uint8_t hh[2];
			hh[0] = FIFO_POP(&time1);
			hh[1] = FIFO_POP(&time1);
			FIFO_POP(&time1);
			uint8_t mm[2];
			mm[0] = FIFO_POP(&time1);
			mm[1] = FIFO_POP(&time1);
			FIFO_POP(&time1);
			uint8_t ss[2];
			ss[0] = FIFO_POP(&time1);
			ss[1] = FIFO_POP(&time1);
			uint8_t hours = (hh[0] - '0') * 10 + (hh[1] - '0');
			uint8_t minutes = (mm[0] - '0') * 10 + (mm[1] - '0');
			uint8_t seconds = (ss[0] - '0') * 10 + (ss[1] - '0');
			if (hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60 && seconds >= 0 && seconds < 60) {
				err_flag = MCP7940M_set_seconds(seconds);
				MCP7940M_set_minutes(minutes);
				MCP7940M_set_hours(hours);
				if (err_flag) {
					UART_put_string(UART_1, &return_status_pos[0]);
					UART_put_string(UART_1, &return_status[0]);
					UART_put_string(UART_1, &succ[0]);
					err_flag = FALSE;
				} else {
					UART_put_string(UART_1, &return_status_pos[0]);
					UART_put_string(UART_1, &return_status[0]);
					UART_put_string(UART_1, &unsucc[0]);
					err_flag = FALSE;
				}
			} else {
				UART_put_string(UART_1, &return_status_pos[0]);
				UART_put_string(UART_1, &return_status[0]);
				UART_put_string(UART_1, &unsucc[0]);
			}
			wait_flag = TRUE;
			UART_put_string(UART_1, &press_any_key_pos[0]);
			UART_put_string(UART_1, &press_any_key[0]);

		}

}
uint8_t SET_TIME_get_exit_flag(time_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case TIME_TERMINAL1:
			return set_time0_exit_flag;
		break;
		case TIME_TERMINAL2:
			return set_time1_exit_flag;
		break;
		default:
			return 0x00;
		break;
	}
}
void SET_TIME_clean_exit_flag(time_profil_t terminal){
	switch ((uint8_t) terminal)
		{
			case TIME_TERMINAL1:
				 set_time0_exit_flag = FALSE;
			break;
			case TIME_TERMINAL2:
				 set_time1_exit_flag = FALSE;
			break;
			default:
			break;
		}
}
