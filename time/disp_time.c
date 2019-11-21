/*
 * disp_time.c
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#include "disp_time.h"

#define DELAYS (2.0F)
#define SYSTEM_CLOCK 21000000U

volatile static fifos_t dtime0, dtime1;
/////////////////////////////////////////start display info///////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tget Time\r" };
static int8_t chat_menu_pos[] = { "\033[15:10H" };
static int8_t chat[] = { "current time time is: " };
static int8_t ret_pos[] = { "\033[16:10H" };
static int8_t inst_pos[] = { "\033[12:10H" };
static int8_t inst[] = { "press [ESC] key to continue....\n\r" };
static int8_t return_status_pos[] = { "\033[17:10H" };
static int8_t return_status[] = { "err getting time\t" };
static int8_t unsucc[] = { "Unsuccessful\n\r" };

static uint8_t disp_time0_exit_flag = FALSE;
static uint8_t disp_time1_exit_flag = FALSE;

void DISP_TIME_display(dtime_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case DTIME_TERMINAL1:
			if (FIFO_init(&dtime0, DTIME_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &title_pos[0]);
				UART_put_string(UART_0, &title[0]);
				UART_put_string(UART_0, &inst_pos[0]);
				UART_put_string(UART_0, &inst[0]);
				UART_put_string(UART_0, &chat_menu_pos[0]);
				UART_put_string(UART_0, &chat[0]);
				disp_time0_exit_flag = FALSE;
				PIT_clock_gating(); //enables pit
				HT16k33_init();
				PIT_enable();

				NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_3); //sets pit priority
				NVIC_global_enable_interrupts;

				PIT_delay(PIT_1, SYSTEM_CLOCK, DELAYS);
				PIT_enable_timer(PIT_1);
				PIT_callback_init(DISP_TIMER0_PIT_handler, PIT_1);
				PIT_enable_interrupt(PIT_1);
			} else {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &return_status_pos[0]);

				UART_put_string(UART_0, &return_status[0]);
				UART_put_string(UART_0, &unsucc[0]);
				disp_time0_exit_flag = FALSE;
			}
		break;
		case DTIME_TERMINAL2:
			if (FIFO_init(&dtime1, DTIME_FIFO_SIZE) == FIFO_SUCCESS) {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &title_pos[0]);
				UART_put_string(UART_1, &title[0]);
				UART_put_string(UART_1, &inst_pos[0]);
				UART_put_string(UART_1, &inst[0]);
				UART_put_string(UART_1, &chat_menu_pos[0]);
				UART_put_string(UART_1, &chat[0]);
				disp_time1_exit_flag = FALSE;
				PIT_clock_gating(); //enables pit
				PIT_enable();
				HT16k33_init();

				NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_3); //sets pit priority
				NVIC_global_enable_interrupts;

				PIT_delay(PIT_2, SYSTEM_CLOCK, DELAYS);
				PIT_enable_timer(PIT_2);
				PIT_callback_init(DISP_TIMER1_PIT_handler, PIT_2);
				PIT_enable_interrupt(PIT_2);
			} else {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &return_status_pos[0]);

				UART_put_string(UART_1, &return_status[0]);
				UART_put_string(UART_1, &unsucc[0]);
				disp_time1_exit_flag = FALSE;
			}

		break;

	}
}

void DISP_TIMER0_PIT_handler(void)
{

	uint8_t seconds = MCP7940M_get_seconds();
	uint8_t minutes = MCP7940M_get_minutes();
	uint8_t hours = MCP7940M_get_hours();
	uint8_t ss[2];
	uint8_t mm[2];
	uint8_t hh[2];

	hh[0] = (hours / 10) + '0';
	hh[1] = (hours % 10) + '0';

	mm[0] = (minutes / 10) + '0';
	mm[1] = (minutes % 10) + '0';

	ss[0] = (seconds / 10) + '0';
	ss[1] = (seconds % 10) + '0';

	if (seconds >= 0 && seconds < 60 && minutes >= 0 && minutes < 60 && hours >= 0 && hours < 24) {
		FIFO_push(&dtime0, hh[0]);
		FIFO_push(&dtime0, hh[1]);
		FIFO_push(&dtime0, ':');
		FIFO_push(&dtime0, mm[0]);
		FIFO_push(&dtime0, mm[1]);
		FIFO_push(&dtime0, ':');
		FIFO_push(&dtime0, ss[0]);
		FIFO_push(&dtime0, ss[1]);
		DISP_TIME_send_to_UART(DTIME_TERMINAL1);
		screen_clear_data();
		screen_add_Item_end(((hours / 10) + '0'));
		screen_add_Item_end(((hours % 10) + '0'));
		screen_add_Item_end(((minutes / 10) + '0'));
		screen_add_Item_end(((minutes % 10) + '0'));
		screen_send_array_2mat();
	} else {
		int8_t err_pos[] = { "\033[16:10H" };
		UART_put_string(UART_0, &err_pos[0]);
		int8_t err[] = { "Something went badly wrong with The RTC" };
		UART_put_string(UART_0, &err[0]);

	}

}

void DISP_TIMER1_PIT_handler(void)
{
	uint8_t seconds = MCP7940M_get_seconds();
	uint8_t minutes = MCP7940M_get_minutes();
	uint8_t hours = MCP7940M_get_hours();
	FIFO_push(&dtime0, ((hours / 10) + '0'));
	FIFO_push(&dtime0, ((hours % 10) + '0'));
	FIFO_push(&dtime0, '/');
	FIFO_push(&dtime0, ((minutes / 10) + '0'));
	FIFO_push(&dtime0, ((minutes % 10) + '0'));
	FIFO_push(&dtime0, '/');
	FIFO_push(&dtime0, ((seconds / 10) + '0'));
	FIFO_push(&dtime0, ((seconds % 10) + '0'));
	DISP_TIME_send_to_UART(DTIME_TERMINAL1);

}
void DISP_TIME_send_to_UART(dtime_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case DTIME_TERMINAL1:
			UART_put_string(UART_0, &ret_pos[0]);
			while (dtime0.status != FIFO_EMPTY) {
				UART_put_char(UART_0, FIFO_POP(&dtime0));
			}

		break;
		case DTIME_TERMINAL2:
			UART_put_string(UART_1, &ret_pos[0]);
			while (dtime1.status != FIFO_EMPTY) {
				UART_put_char(UART_1, FIFO_POP(&dtime1));
			}

		break;
	}

}
void DISP_TIME_uart0_handler(void)
{
	uint8_t data_recived = UART_get_mailbox(UART_0);
	if (data_recived == '\e'){
		disp_time0_exit_flag = TRUE;
		PIT_disable_timer(PIT_1);
		screen_clear_data();
		screen_send_array_2mat();

	}

}
void DISP_TIME_uart1_handler(void)
{
	uint8_t data_recived = UART_get_mailbox(UART_1);
	if (data_recived == '\e'){
		disp_time1_exit_flag = TRUE;
		PIT_disable_timer(PIT_2);
		screen_clear_data();
		screen_send_array_2mat();

	}
}
uint8_t DISP_TIME_get_exit_flag(dtime_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case DTIME_TERMINAL1:
			if (disp_time0_exit_flag)
//				PIT_disable_timer(PIT_1);
			    return disp_time0_exit_flag;
		break;
		case DTIME_TERMINAL2:
			if (disp_time1_exit_flag) PIT_disable_timer(PIT_2);
			return disp_time1_exit_flag;
		break;
		default:

		break;
	}
	return FALSE;
}
void DISP_TIME_clean_exit_flag(dtime_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case DTIME_TERMINAL1:

			disp_time0_exit_flag = FALSE;
		break;
		case DTIME_TERMINAL2:
			disp_time1_exit_flag = FALSE;
		break;
		default:
		break;
	}
}
