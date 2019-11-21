/*
 * set_time.c
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#include <msg/set_msg.h>
/////////////////////////////////////////start display info///////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tset MSG\r" };
static int8_t chat_menu_pos[] = { "\033[12:10H" };
static int8_t chat[] = { "message that you want to change from 1 to 5:\r\n" };
static int8_t return_status_pos[] = { "\033[13:10H" };
static int8_t return_status[] = { "message change was:\t" };
static int8_t unsucc[] = { "Unsuccessful\n\r" };
static int8_t succ[] = { "successful\n\r" };
static int8_t press_any_key[] = { "press any key to continue....\n\r" };
static int8_t selected[] = { "[SELECTED]\n\r" };

static volatile uint8_t set_msg0_exit_flag = FALSE;
static volatile uint8_t set_msg1_exit_flag = FALSE;

static volatile uint8_t msg_select1_f = FALSE;
static volatile uint8_t msg_select2_f = FALSE;

static fifos_t msg1, msg2, msg3, msg4, msg5;

static uint8_t g_limit_reached = FALSE;

void SET_MSG_display(msg_profil_t terminal)
{
	g_limit_reached = FALSE;
	switch ((uint8_t) terminal)
	{
		case MSG_TERMINAL1:
			if (FIFO_init(&msg1, MSG_FIFO_SIZE) == FIFO_SUCCESS) {
				FIFO_init(&msg2, MSG_FIFO_SIZE);
				FIFO_init(&msg3, MSG_FIFO_SIZE);
				FIFO_init(&msg4, MSG_FIFO_SIZE);
				FIFO_init(&msg5, MSG_FIFO_SIZE);
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &title_pos[0]);
				UART_put_string(UART_0, &title[0]);
				UART_put_string(UART_0, &chat_menu_pos[0]);
				UART_put_string(UART_0, &chat[0]);
				set_msg0_exit_flag = FALSE;
				msg_select1_f = FALSE;
			} else {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &return_status_pos[0]);

				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &unsucc[0]);
				set_msg0_exit_flag = FALSE;
				msg_select1_f = FALSE;
			}
		break;
		case MSG_TERMINAL2:
			if (FIFO_init(&msg1, MSG_FIFO_SIZE) == FIFO_SUCCESS) {
				FIFO_init(&msg2, MSG_FIFO_SIZE);
				FIFO_init(&msg3, MSG_FIFO_SIZE);
				FIFO_init(&msg4, MSG_FIFO_SIZE);
				FIFO_init(&msg5, MSG_FIFO_SIZE);
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &title_pos[0]);
				UART_put_string(UART_1, &title[0]);
				UART_put_string(UART_1, &chat_menu_pos[0]);
				UART_put_string(UART_1, &chat[0]);
				set_msg1_exit_flag = FALSE;
				msg_select2_f = FALSE;
			} else {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &return_status_pos[0]);

				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &unsucc[0]);
				set_msg1_exit_flag = FALSE;
				msg_select2_f = FALSE;
			}

		break;

	}

}
void SET_MSG_uart0_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
	//static volatile uint8_t err_flag = FALSE;
	static volatile uint8_t msg_2_change = 0x00;
	uint8_t data_recived = UART_get_mailbox(UART_0);
	if (wait_flag == TRUE) {
		set_msg0_exit_flag = TRUE;
		wait_flag = FALSE;
		msg_2_change = 0x00;
	} else if (data_recived != '\e' && data_recived != '\r') {
		if (!msg_select1_f) {
			if (data_recived > '0' && data_recived <= '5') {
				UART_put_char(UART_0, data_recived);
				UART_put_string(UART_0, &selected[0]);
				UART_put_char(UART_0, '\r');
				UART_put_char(UART_0, '\n');
				msg_2_change = data_recived;
				msg_select1_f = TRUE;
			}
		} else {
			switch (msg_2_change)
			{
				case MSG_1:
					if (FIFO_push(&msg1, data_recived) == FIFO_FULL)
						g_limit_reached = TRUE;
				break;
				case MSG_2:
					if (FIFO_push(&msg2 , data_recived) == FIFO_FULL)
						g_limit_reached = TRUE;
				break;
				case MSG_3:
					if (FIFO_push(&msg3 , data_recived) == FIFO_FULL)
						g_limit_reached = TRUE;
				break;
				case MSG_4:
					if (FIFO_push(&msg4 , data_recived) == FIFO_FULL)
						g_limit_reached = TRUE;
				break;
				case MSG_5:
					if (FIFO_push(&msg5 , data_recived) == FIFO_FULL)
						g_limit_reached = TRUE;
				break;
				default:
				break;

			}
			UART_put_char(UART_0, data_recived);
		}

	}
	if ((data_recived == '\r' && msg_select1_f != FALSE && wait_flag == FALSE) || g_limit_reached == TRUE) {
		uint8_t err = 0x00;
		switch (msg_2_change)
		{
			case MSG_1:
				M24LC256_write_sequential(&msg1.data[0], msg1.max_size + 1, MSG1_MEM);
				UART_put_char(UART_0, '\n');
				UART_put_char(UART_0, '\r');
				UART_put_string(UART_0, &return_status[0]);
				if (!err) {
					UART_put_string(UART_0, &succ[0]);
				} else {
					UART_put_string(UART_0, &unsucc[0]);
				}
				UART_put_string(UART_0, &press_any_key[0]);
				wait_flag = TRUE;
			break;
			case MSG_2:
				M24LC256_write_sequential(&msg2.data[0], msg2.max_size + 1, MSG2_MEM);
				UART_put_char(UART_0, '\n');
				UART_put_char(UART_0, '\r');
				UART_put_string(UART_0, &return_status[0]);
				if (!err) {
					UART_put_string(UART_0, &succ[0]);
				} else {
					UART_put_string(UART_0, &unsucc[0]);
				}
				UART_put_string(UART_0, &press_any_key[0]);
				wait_flag = TRUE;
			break;
			case MSG_3:
				M24LC256_write_sequential(&msg3.data[0], msg3.max_size + 1, MSG3_MEM);
				UART_put_char(UART_0, '\n');
				UART_put_char(UART_0, '\r');
				UART_put_string(UART_0, &return_status[0]);
				if (!err) {
					UART_put_string(UART_0, &succ[0]);
				} else {
					UART_put_string(UART_0, &unsucc[0]);
				}
				UART_put_string(UART_0, &press_any_key[0]);
				wait_flag = TRUE;
			break;
			case MSG_4:
				M24LC256_write_sequential(&msg4.data[0], msg4.max_size + 1, MSG4_MEM);
				UART_put_char(UART_0, '\n');
				UART_put_char(UART_0, '\r');
				UART_put_string(UART_0, &return_status[0]);
				if (!err) {
					UART_put_string(UART_0, &succ[0]);
				} else {
					UART_put_string(UART_0, &unsucc[0]);
				}
				UART_put_string(UART_0, &press_any_key[0]);
				wait_flag = TRUE;
			break;
			case MSG_5:
				M24LC256_write_sequential(&msg5.data[0], msg5.max_size + 1, MSG5_MEM);
				UART_put_char(UART_0, '\n');
				UART_put_char(UART_0, '\r');
				UART_put_string(UART_0, &return_status[0]);
				if (!err) {
					UART_put_string(UART_0, &succ[0]);
				} else {
					UART_put_string(UART_0, &unsucc[0]);
				}
				UART_put_string(UART_0, &press_any_key[0]);
				wait_flag = TRUE;
			break;
			default:
			break;

		}
	} else if (data_recived == '\e') {
		set_msg0_exit_flag = TRUE;
	}


}

void SET_MSG_uart1_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
		//static volatile uint8_t err_flag = FALSE;
		static volatile uint8_t msg_2_change = 0x00;
		uint8_t data_recived = UART_get_mailbox(UART_1);
		if (wait_flag == TRUE) {
			set_msg1_exit_flag = TRUE;
			wait_flag = FALSE;
			msg_2_change = 0x00;
		} else if (data_recived != '\e' && data_recived != '\r') {
			if (!msg_select2_f) {
				if (data_recived > '0' && data_recived <= '5') {
					UART_put_char(UART_1, data_recived);
					UART_put_string(UART_1, &selected[0]);
					UART_put_char(UART_1, '\r');
					UART_put_char(UART_1, '\n');
					msg_2_change = data_recived;
					msg_select2_f = TRUE;
				}
			} else {
				switch (msg_2_change)
				{
					case MSG_1:
						if (FIFO_push(&msg1, data_recived) == FIFO_FULL)
							g_limit_reached = TRUE;
					break;
					case MSG_2:
						if (FIFO_push(&msg2 , data_recived) == FIFO_FULL)
							g_limit_reached = TRUE;
					break;
					case MSG_3:
						if (FIFO_push(&msg3 , data_recived) == FIFO_FULL)
							g_limit_reached = TRUE;
					break;
					case MSG_4:
						if (FIFO_push(&msg4 , data_recived) == FIFO_FULL)
							g_limit_reached = TRUE;
					break;
					case MSG_5:
						if (FIFO_push(&msg5 , data_recived) == FIFO_FULL)
							g_limit_reached = TRUE;
					break;
					default:
					break;

				}
				UART_put_char(UART_1, data_recived);
			}

		}
		if ((data_recived == '\r' && msg_select2_f != FALSE && wait_flag == FALSE) || g_limit_reached == TRUE) {
			uint8_t err = 0x00;
			switch (msg_2_change)
			{
				case MSG_1:
					M24LC256_write_sequential(&msg1.data[0], msg1.max_size + 1, MSG1_MEM);
					UART_put_char(UART_1, '\n');
					UART_put_char(UART_1, '\r');
					UART_put_string(UART_1, &return_status[0]);
					if (!err) {
						UART_put_string(UART_1, &succ[0]);
					} else {
						UART_put_string(UART_1, &unsucc[0]);
					}
					UART_put_string(UART_1, &press_any_key[0]);
					wait_flag = TRUE;
				break;
				case MSG_2:
					M24LC256_write_sequential(&msg2.data[0], msg2.max_size + 1, MSG2_MEM);
					UART_put_char(UART_1, '\n');
					UART_put_char(UART_1, '\r');
					UART_put_string(UART_1, &return_status[0]);
					if (!err) {
						UART_put_string(UART_1, &succ[0]);
					} else {
						UART_put_string(UART_1, &unsucc[0]);
					}
					UART_put_string(UART_1, &press_any_key[0]);
					wait_flag = TRUE;
				break;
				case MSG_3:
					M24LC256_write_sequential(&msg3.data[0], msg3.max_size + 1, MSG3_MEM);
					UART_put_char(UART_1, '\n');
					UART_put_char(UART_1, '\r');
					UART_put_string(UART_1, &return_status[0]);
					if (!err) {
						UART_put_string(UART_1, &succ[0]);
					} else {
						UART_put_string(UART_1, &unsucc[0]);
					}
					UART_put_string(UART_1, &press_any_key[0]);
					wait_flag = TRUE;
				break;
				case MSG_4:
					M24LC256_write_sequential(&msg4.data[0], msg4.max_size + 1, MSG4_MEM);
					UART_put_char(UART_1, '\n');
					UART_put_char(UART_1, '\r');
					UART_put_string(UART_1, &return_status[0]);
					if (!err) {
						UART_put_string(UART_1, &succ[0]);
					} else {
						UART_put_string(UART_1, &unsucc[0]);
					}
					UART_put_string(UART_1, &press_any_key[0]);
					wait_flag = TRUE;
				break;
				case MSG_5:
					M24LC256_write_sequential(&msg5.data[0], msg5.max_size + 1, MSG5_MEM);
					UART_put_char(UART_1, '\n');
					UART_put_char(UART_1, '\r');
					UART_put_string(UART_1, &return_status[0]);
					if (!err) {
						UART_put_string(UART_1, &succ[0]);
					} else {
						UART_put_string(UART_1, &unsucc[0]);
					}
					UART_put_string(UART_1, &press_any_key[0]);
					wait_flag = TRUE;
				break;
				default:
				break;

			}
		} else if (data_recived == '\e') {
			set_msg1_exit_flag = TRUE;
		}
}
uint8_t SET_MSG_get_exit_flag(msg_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case MSG_TERMINAL1:
			return set_msg0_exit_flag;
		break;
		case MSG_TERMINAL2:
			return set_msg1_exit_flag;
		break;
		default:
			return 0x00;
		break;
	}
}
void SET_MSG_clean_exit_flag(msg_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case MSG_TERMINAL1:
			set_msg0_exit_flag = FALSE;
		break;
		case MSG_TERMINAL2:
			set_msg1_exit_flag = FALSE;
		break;
		default:
		break;
	}
}
