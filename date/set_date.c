/*
 * set_time.c
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#include <date/set_date.h>
/////////////////////////////////////////start display info///////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tset Date\r" };
static int8_t chat_menu_pos[] = { "\033[12:10H" };
static int8_t chat[] = { "Write date in \"dd/mm/yy\" format\r\n" };
static int8_t return_status_pos[] = { "\033[13:10H" };
static int8_t return_status[] = { "date change was:\t" };
static int8_t unsucc[] = { "Unsuccessful\n\r" };
static int8_t succ[] = { "successful\n\r" };
static int8_t press_any_key_pos[] = { "\033[15:10H" };
static int8_t press_any_key[] = { "press any key to continue....\n\r" };

uint8_t set_date0_exit_flag = FALSE;
uint8_t set_date1_exit_flag = FALSE;

static fifos_t date0, date1;

void SET_DATE_display(date_profil_t terminal)
{

	switch ((uint8_t) terminal)
	{
		case DATE_TERMINAL1:
			if (FIFO_init(&date0, DATE_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &title_pos[0]);
				UART_put_string(UART_0, &title[0]);
				UART_put_string(UART_0, &chat_menu_pos[0]);
				UART_put_string(UART_0, &chat[0]);
				set_date0_exit_flag = FALSE;
			} else {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &return_status_pos[0]);

				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &unsucc[0]);
				set_date0_exit_flag = FALSE;
			}
		break;
		case DATE_TERMINAL2:
			if (FIFO_init(&date1, DATE_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &title_pos[0]);
				UART_put_string(UART_1, &title[0]);
				UART_put_string(UART_1, &chat_menu_pos[0]);
				UART_put_string(UART_1, &chat[0]);
				set_date0_exit_flag = FALSE;
			} else {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &return_status_pos[0]);

				UART_put_string(UART_1, &return_status[0]);
				UART_put_string(UART_1, &unsucc[0]);
				set_date0_exit_flag = FALSE;
			}
		break;

	}

}
void SET_DATE_uart0_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
	static volatile uint8_t err_flag = FALSE;
	uint8_t data_recived = UART_get_mailbox(UART_0);
	if (wait_flag == TRUE) {
		set_date0_exit_flag = TRUE;
		wait_flag = FALSE;
		err_flag = FALSE;
	} else if (data_recived != '\e' && data_recived != '\r' && FIFO_getStatus(&date0) != FIFO_FULL) {
		FIFO_push(&date0, data_recived);
		UART_put_char(UART_0, data_recived);
	} else if (data_recived == '\r' && date0.index < (DATE_FIFO_SIZE - 1)) {
		UART_put_string(UART_0, &return_status_pos[0]);
		UART_put_string(UART_0, &return_status[0]);
		UART_put_string(UART_0, &unsucc[0]);
		wait_flag = TRUE;
		UART_put_string(UART_0, &press_any_key_pos[0]);
		UART_put_string(UART_0, &press_any_key[0]);
	} else if (data_recived == '\e') {
		set_date0_exit_flag = TRUE;
	} else if (date0.status == FIFO_FULL) {
		uint8_t dd[2];
		dd[0] = FIFO_POP(&date0);
		dd[1] = FIFO_POP(&date0);
		FIFO_POP(&date0);
		uint8_t mm[2];
		mm[0] = FIFO_POP(&date0);
		mm[1] = FIFO_POP(&date0);
		FIFO_POP(&date0);
		uint8_t aa[2];
		aa[0] = FIFO_POP(&date0);
		aa[1] = FIFO_POP(&date0);
		uint8_t days = (dd[0] - '0') * 10 + (dd[1] - '0');
		uint8_t month = (mm[0] - '0') * 10 + (mm[1] - '0');
		uint8_t year = (aa[0] - '0') * 10 + (aa[1] - '0');
		if (days > 0 && days <= 31 && month > 0 && month <= 12 && year >= 0 && year <= 99) {
			err_flag = MCP7940M_set_date(days, month, year);

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

void SET_DATE_uart1_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
	static volatile uint8_t err_flag = FALSE;
	uint8_t data_recived = UART_get_mailbox(UART_1);
	if (wait_flag == TRUE) {
		set_date1_exit_flag = TRUE;
		wait_flag = FALSE;
		err_flag = FALSE;
	} else if (data_recived != '\e' && data_recived != '\r' && FIFO_getStatus(&date1) != FIFO_FULL) {
		FIFO_push(&date1, data_recived);
		UART_put_char(UART_1, data_recived);
	} else if (data_recived == '\r' && date1.index < (DATE_FIFO_SIZE - 1)) {
		UART_put_string(UART_1, &return_status_pos[0]);
		UART_put_string(UART_1, &return_status[0]);
		UART_put_string(UART_1, &unsucc[0]);
		wait_flag = TRUE;
		UART_put_string(UART_1, &press_any_key_pos[0]);
		UART_put_string(UART_1, &press_any_key[0]);
	} else if (data_recived == '\e') {
		set_date1_exit_flag = TRUE;
	} else if (date1.status == FIFO_FULL) {
		uint8_t dd[2];
		dd[0] = FIFO_POP(&date1);
		dd[1] = FIFO_POP(&date1);
		FIFO_POP(&date1);
		uint8_t mm[2];
		mm[0] = FIFO_POP(&date1);
		mm[1] = FIFO_POP(&date1);
		FIFO_POP(&date1);
		uint8_t aa[2];
		aa[0] = FIFO_POP(&date1);
		aa[1] = FIFO_POP(&date1);
		uint8_t days = (dd[0] - '0') * 10 + (dd[1] - '0');
		uint8_t month = (mm[0] - '0') * 10 + (mm[1] - '0');
		uint8_t year = (aa[0] - '0') * 10 + (aa[1] - '0');
		if (days >= 0 && days < 24 && month >= 0 && month < 60 && year >= 0 && year < 60) {
			err_flag = MCP7940M_set_date(days, month, year);

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
uint8_t SET_DATE_get_exit_flag(date_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case DATE_TERMINAL1:
			return set_date0_exit_flag;
		break;
		case DATE_TERMINAL2:
			return set_date1_exit_flag;
		break;
		default:
			return 0x00;
		break;
	}
}
void SET_DATE_clean_exit_flag(date_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case DATE_TERMINAL1:
			set_date0_exit_flag = FALSE;
		break;
		case DATE_TERMINAL2:
			set_date1_exit_flag = FALSE;
		break;
		default:
		break;
	}
}
