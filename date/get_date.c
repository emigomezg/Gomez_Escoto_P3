/*
 * get_time.c
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#include <date/get_date.h>
#define DELAYS (0.5F)
#define SYSTEM_CLOCK 21000000U

static fifos_t date0, date1;
/////////////////////////////////////////start display info///////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tget Datee\r" };
static int8_t chat_menu_pos[] = { "\033[15:10H" };
static int8_t chat[] = { "current time time is: " };
static int8_t ret_pos[] = { "\033[16:10H" };
static int8_t inst_pos[] = { "\033[13:10H" };
static int8_t inst[] = { "press [ESC] key to continue....\n\r" };
static int8_t return_status_pos[] = { "\033[18:10H" };
static int8_t return_status[] = { "err getting time\t" };
static int8_t unsucc[] = { "Unsuccessful\n\r" };

static uint8_t get_date0_exit_flag = FALSE;
static uint8_t get_date1_exit_flag = FALSE;

void GET_DATE_display(gdate_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case GDATE_TERMINAL0:
			if (FIFO_init(&date0, DATE_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &title_pos[0]);
				UART_put_string(UART_0, &title[0]);
				UART_put_string(UART_0, &inst_pos[0]);
				UART_put_string(UART_0, &inst[0]);
				UART_put_string(UART_0, &chat_menu_pos[0]);
				UART_put_string(UART_0, &chat[0]);
				get_date0_exit_flag = FALSE;
				PIT_clock_gating(); //enables pit
				PIT_enable();

				NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_3); //sets pit priority
				NVIC_global_enable_interrupts;

				PIT_delay(PIT_1, SYSTEM_CLOCK, DELAYS);
				PIT_enable_timer(PIT_1);
				PIT_callback_init(GET_DATE0_PIT_handler, PIT_1);
				PIT_enable_interrupt(PIT_1);
			} else {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &return_status_pos[0]);

				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &unsucc[0]);
				get_date0_exit_flag = FALSE;
			}
		break;
		case GDATE_TERMINAL1:
			if (FIFO_init(&date1, DATE_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &title_pos[0]);
				UART_put_string(UART_1, &title[0]);
				UART_put_string(UART_1, &inst_pos[0]);
				UART_put_string(UART_1, &inst[0]);
				UART_put_string(UART_1, &chat_menu_pos[0]);
				UART_put_string(UART_1, &chat[0]);
				get_date1_exit_flag = FALSE;
				PIT_clock_gating(); //enables pit
				PIT_enable();

				NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_3); //sets pit priority
				NVIC_global_enable_interrupts;

				PIT_delay(PIT_2, SYSTEM_CLOCK, DELAYS);
				PIT_enable_timer(PIT_2);
				PIT_callback_init(GET_DATE1_PIT_handler, PIT_2);
				PIT_enable_interrupt(PIT_2);
			} else {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &return_status_pos[0]);

				UART_put_string(UART_1, &return_status[0]);
				UART_put_string(UART_1, &unsucc[0]);
				get_date1_exit_flag = FALSE;
			}

		break;

	}
}

void GET_DATE0_PIT_handler(void)
{uint8_t day = MCP7940M_get_day();
uint8_t month = MCP7940M_get_month();
uint8_t year = MCP7940M_get_year();
FIFO_push(&date0, (day / 10) + '0');
FIFO_push(&date0, (day % 10) + '0');
FIFO_push(&date0, '/');
FIFO_push(&date0, (month / 10) + '0');
FIFO_push(&date0, (month % 10) + '0');
FIFO_push(&date0, '/');
FIFO_push(&date0, (year / 10) + '0');
FIFO_push(&date0, (year % 10) + '0');
	GET_DATE_send_to_UART(GDATE_TERMINAL0);
	PIT_disable_timer(PIT_1);

}
void GET_DATE1_PIT_handler(void)
{
	uint8_t day = MCP7940M_get_day();
	uint8_t month = MCP7940M_get_month();
	uint8_t year = MCP7940M_get_year();
	FIFO_push(&date1, (day / 10) + '0');
	FIFO_push(&date1, (day % 10) + '0');
	FIFO_push(&date1, '/');
	FIFO_push(&date1, (month / 10) + '0');
	FIFO_push(&date1, (month % 10) + '0');
	FIFO_push(&date1, '/');
	FIFO_push(&date1, (year / 10) + '0');
	FIFO_push(&date1, (year % 10) + '0');
	GET_DATE_send_to_UART(GDATE_TERMINAL1);
	PIT_disable_timer(PIT_2);

}

void GET_DATE_send_to_UART(gdate_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case GDATE_TERMINAL0:
			UART_put_string(UART_0, &ret_pos[0]);
			while (date0.status != FIFO_EMPTY) {
				UART_put_char(UART_0, FIFO_POP(&date0));
			}

		break;
		case GDATE_TERMINAL1:
			UART_put_string(UART_1, &ret_pos[0]);
			while (date1.status != FIFO_EMPTY) {
				UART_put_char(UART_1, FIFO_POP(&date1));
			}

		break;
	}

}
void GET_DATE_uart0_handler(void)
{
	uint8_t data_recived = UART_get_mailbox(UART_0);
	if (data_recived == '\e') {
		get_date0_exit_flag = TRUE;
	}

}
void GET_DATE_uart1_handler(void)
{
	uint8_t data_recived = UART_get_mailbox(UART_1);
	if (data_recived == '\e') {
		get_date1_exit_flag = TRUE;
	}
}
uint8_t GET_DATE_get_exit_flag(gdate_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case GDATE_TERMINAL0:
			if (get_date0_exit_flag) PIT_disable_timer(PIT_1);
			return get_date0_exit_flag;
		break;
		case GDATE_TERMINAL1:
			if (get_date1_exit_flag) PIT_disable_timer(PIT_2);
			return get_date1_exit_flag;
		break;
		default:
			return FALSE;
		break;
	}

}
void GET_DATE_clean_exit_flag(gdate_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case GDATE_TERMINAL0:

			 get_date0_exit_flag=FALSE;
		break;
		case GDATE_TERMINAL1:
			 get_date1_exit_flag =FALSE;
		break;
		default:
		break;
	}
}
