/*
 * set_time.c
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#include <chat/chat.h>

#define DELAYS (0.5F)
#define SYSTEM_CLOCK 21000000U

/////////////////////////////////////////start display info///////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tCHAT\r\r\n" };
//static int8_t chat_menu_pos[] = { "\033[12:10H" };
static int8_t TER1_online[] = { "[TERMINAL 1 ONLINE]\r\n" };
static int8_t TER2_online[] = { "[TERMINAL 2 ONLINE]\r\n" };
static int8_t TER1_offline[] = { "[TERMINAL 1 OFFLINE]\r\n" };
static int8_t TER2_offline[] = { "[TERMINAL 2 OFFLINE]\r\n" };
//static int8_t terminal_status_pos[] = { "\033[11:10H" };

static int8_t terminal_online_pos[] = { "\033[11:10H" };
static int8_t terminal_response[] = { "\033[14:10H" };
static int8_t write_response[] = { "\033[18:10H" };
static int8_t terminal1_responded[] = { "[TERMINAL 1] :\t" };
static int8_t terminal2_responded[] = { "[TERMINAL 2] :\t" };

static volatile uint8_t chat1_exit_flag = FALSE;
static volatile uint8_t chat2_exit_flag = FALSE;

static volatile uint8_t msg_select1_f = FALSE;
static volatile uint8_t msg_select2_f = FALSE;

static volatile uint8_t is_term1_online = FALSE;
static volatile uint8_t is_term2_online = FALSE;

static fifos_t chat1_RX, chat1_TX;
static fifos_t chat2_RX, chat2_TX;

static uint8_t g_limit_reached = FALSE;

void CHAT_display(chat_profil_t terminal)
{
	g_limit_reached = FALSE;
	switch ((uint8_t) terminal)
	{
		case CHAT_TERMINAL1:
			FIFO_init(&chat1_TX, CHAT_FIFO_SIZE);
			FIFO_init(&chat1_RX, CHAT_FIFO_SIZE);
			is_term1_online = TRUE;
			UART_put_string(UART_0, &clean_screen_all[0]);
			UART_put_string(UART_0, &clean_screen[0]);
			UART_put_string(UART_0, &red_back_yellow[0]);
			UART_put_string(UART_0, &title_pos[0]);
			UART_put_string(UART_0, &title[0]);
			chat1_exit_flag = FALSE;

		break;
		case CHAT_TERMINAL2:
			FIFO_init(&chat2_TX, CHAT_FIFO_SIZE);
			FIFO_init(&chat2_RX, CHAT_FIFO_SIZE);
			is_term2_online = TRUE;
			UART_put_string(UART_1, &clean_screen_all[0]);
			UART_put_string(UART_1, &clean_screen[0]);
			UART_put_string(UART_1, &red_back_yellow[0]);
			UART_put_string(UART_1, &title_pos[0]);
			UART_put_string(UART_1, &title[0]);
			chat2_exit_flag = FALSE;
		break;

	}

}

void CHAT_uart0_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
	static volatile uint8_t first_online = TRUE;
	uint8_t data_recived = UART_get_mailbox(UART_0);
	if (wait_flag == TRUE) {
		chat1_exit_flag = TRUE;
		first_online = TRUE;
		wait_flag = FALSE;
	} else if (data_recived != '\e' && data_recived != '\r') {
		if (is_term2_online) {
			if (first_online) {
				UART_put_string(UART_0, &terminal_online_pos[0]);
				UART_put_string(UART_0, &TER2_online[0]);
				UART_put_string(UART_0, &write_response[0]);
				first_online = FALSE;
			}

			if (chat1_TX.status == FIFO_EMPTY) {
				UART_put_string(UART_0, &write_response[0]);
			}
			UART_put_char(UART_0, data_recived);

			FIFO_push(&chat1_TX, data_recived);
			if (chat1_RX.status == FIFO_FULL) {
				//data sended
				UART_put_string(UART_1, &terminal_response[0]);
				UART_put_string(UART_1, &terminal1_responded[0]);
				while (chat1_TX.status == FIFO_EMPTY)
					UART_put_char(UART_1, FIFO_POP(&chat1_TX));
			}
		} else {
			UART_put_string(UART_0, &terminal_online_pos[0]);
			UART_put_string(UART_0, &TER2_offline[0]);
		}
	} else if (data_recived == '\r' && chat1_TX.status != FIFO_EMPTY) {
		UART_put_string(UART_1, &terminal_response[0]);
		UART_put_string(UART_1, &terminal1_responded[0]);
		uint8_t index = chat1_TX.index;
		while (chat1_TX.index >= 1)
			UART_put_char(UART_1, FIFO_POP(&chat1_TX));
		UART_put_char(UART_0, '\r');
		while (index) {
			UART_put_char(UART_0, ' ');
			index--;
		}
		UART_put_char(UART_0, '\r');

	} else if (data_recived == '\e') {
		chat1_exit_flag = TRUE;
		is_term1_online = FALSE;
		wait_flag = FALSE;
		first_online = FALSE;
	}
}

void CHAT_uart1_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
	static volatile uint8_t first_online = TRUE;
	uint8_t data_recived = UART_get_mailbox(UART_1);
	if (wait_flag == TRUE) {
		chat2_exit_flag = TRUE;
		first_online = TRUE;
		wait_flag = FALSE;
	} else if (data_recived != '\e' && data_recived != '\r') {
		if (is_term1_online) {
			if (first_online) {
				UART_put_string(UART_1, &terminal_online_pos[0]);
				UART_put_string(UART_1, &TER1_online[0]);
				UART_put_string(UART_1, &write_response[0]);
				first_online = FALSE;
			}
			if (chat2_TX.status == FIFO_EMPTY) {
				UART_put_string(UART_1, &write_response[0]);
			}
			UART_put_char(UART_1, data_recived);
			FIFO_push(&chat2_TX, data_recived);
			if (chat2_RX.status == FIFO_FULL) {
				//data sended
				UART_put_string(UART_0, &terminal_response[0]);
				UART_put_string(UART_0, &terminal2_responded[0]);
				while (chat2_TX.status == FIFO_EMPTY)
					UART_put_char(UART_0, FIFO_POP(&chat2_TX));
			}
		} else {
			UART_put_string(UART_1, &terminal_online_pos[0]);
			UART_put_string(UART_1, &TER1_offline[0]);
		}
	} else if (data_recived == '\r' && chat2_TX.status != FIFO_EMPTY) {
		UART_put_string(UART_0, &terminal_response[0]);
		UART_put_string(UART_0, &terminal2_responded[0]);
		uint8_t index = chat2_TX.index;
		while (chat2_TX.index >= 1)
			UART_put_char(UART_0, FIFO_POP(&chat2_TX));
		UART_put_char(UART_1, '\r');
		while (index) {
			UART_put_char(UART_1, ' ');
			index--;
		}
		UART_put_char(UART_1, '\r');

	} else if (data_recived == '\e') {
		chat2_exit_flag = TRUE;
		is_term2_online = FALSE;
		wait_flag = FALSE;
		first_online = FALSE;
	}
}
uint8_t CHAT_get_exit_flag(chat_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case CHAT_TERMINAL1:
			return chat1_exit_flag;
		break;
		case CHAT_TERMINAL2:
			return chat2_exit_flag;
		break;
		default:
			return 0x00;
		break;
	}
}
void CHAT_clean_exit_flag(chat_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case CHAT_TERMINAL1:
			chat1_exit_flag = FALSE;
		break;
		case CHAT_TERMINAL2:
			chat2_exit_flag = FALSE;
		break;
		default:
		break;
	}
}
