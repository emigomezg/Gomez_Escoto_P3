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
static int8_t press_any_key_pos[] = { "\033[15:10H" };
static int8_t press_any_key[] = { "press any key to continue....\n\r" };
static int8_t selected[] = { "[SELECTED]\n\r" };


static volatile uint8_t set_msg0_exit_flag = FALSE;
static volatile uint8_t set_msg1_exit_flag = FALSE;

static volatile uint8_t msg_select_f = FALSE;

static fifos_t msg1, msg2,msg3,msg4,msg5;

void SET_MSG_display(msg_profil_t terminal)
{

	switch ((uint8_t) terminal)
	{
		case MSG_TERMINAL0:
			if (FIFO_init(&msg1, MSG_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &title_pos[0]);
				UART_put_string(UART_0, &title[0]);
				UART_put_string(UART_0, &chat_menu_pos[0]);
				UART_put_string(UART_0, &chat[0]);
				set_msg0_exit_flag = FALSE;
				msg_select_f = FALSE;
			} else {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &return_status_pos[0]);

				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &unsucc[0]);
				set_msg0_exit_flag = FALSE;
				msg_select_f = FALSE;
			}
		break;
		case MSG_TERMINAL1:
			if (FIFO_init(&msg1, MSG_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &title_pos[0]);
				UART_put_string(UART_1, &title[0]);
				UART_put_string(UART_1, &chat_menu_pos[0]);
				UART_put_string(UART_1, &chat[0]);
				set_msg1_exit_flag = FALSE;
				msg_select_f=FALSE;
			} else {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &return_status_pos[0]);

				UART_put_string(UART_1, &return_status[0]);
				UART_put_string(UART_1, &unsucc[0]);
				set_msg1_exit_flag = FALSE;
				msg_select_f =FALSE;
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
	}
	if (data_recived != '\e' && data_recived != '\r') {
		if(!msg_select_f){
			UART_put_char(UART_0, data_recived);
			UART_put_string(UART_0, &selected[0]);
			UART_put_char(UART_0, '\r');
			UART_put_char(UART_0, '\n');
			msg_2_change = data_recived;
			msg_select_f = TRUE;
		}else
		{
			switch(msg_2_change)
			{
				case MSG_1:
					FIFO_push(&msg1, data_recived);
					break;
				case MSG_2:
					FIFO_push(&msg2, data_recived);
					break;
				case MSG_3:
					FIFO_push(&msg3, data_recived);
					break;
				case MSG_4:
					FIFO_push(&msg4, data_recived);
					break;
				case MSG_5:
					FIFO_push(&msg5, data_recived);
					break;
				default:
					break;

			}
			UART_put_char(UART_0, data_recived);
		}

	} else if (data_recived == '\r'&& msg_select_f!=FALSE) {
		uint8_t err;
		switch(msg_2_change)
		{
			case MSG_1:
				for(uint8_t a = 0; FIFO_EMPTY!=msg1.status;a++){
					err = M24LC256_Write_random(FIFO_POP(&msg1), MSG1_MEM+a);
				}
				UART_put_char(UART_0,'\n');
				UART_put_char(UART_0,'\r');
				UART_put_string(UART_0, &return_status[0]);
				if(!err){
					UART_put_string(UART_0, &succ[0]);
				}else{
					UART_put_string(UART_0, &unsucc[0]);
				}
				UART_put_string(UART_0, &press_any_key[0]);
				wait_flag = TRUE;
				break;
			case MSG_2:
				break;
			case MSG_3:
				break;
			case MSG_4:
				break;
			case MSG_5:
				break;
			default:
				break;

		}
	} else if (data_recived == '\e') {
		set_msg0_exit_flag = TRUE;
	} else if (msg1.status == FIFO_FULL) {

			/*err_flag = MCP7940M_set_seconds(seconds);
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
		}*/
		wait_flag = TRUE;
		UART_put_string(UART_0, &press_any_key_pos[0]);
		UART_put_string(UART_0, &press_any_key[0]);

	}

}

void SET_MSG_uart1_handler(void)
{

}
uint8_t SET_MSG_get_exit_flag(msg_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case MSG_TERMINAL0:
			return set_msg0_exit_flag;
		break;
		case MSG_TERMINAL1:
			return set_msg1_exit_flag;
		break;
		default:
			return 0x00;
		break;
	}
}
void SET_MSG_clean_exit_flag(msg_profil_t terminal){
	switch ((uint8_t) terminal)
		{
			case MSG_TERMINAL0:
				 set_msg0_exit_flag = FALSE;
			break;
			case MSG_TERMINAL1:
				 set_msg1_exit_flag = FALSE;
			break;
			default:
			break;
		}
}
