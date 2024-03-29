/*
 * PSM.c
 *
 *  Created on: Nov 17, 2019
 *      Author: alvar
 */

#include "PSM.h"

#define DELAYS (1.0F)

#define CLK_K64 21000000U
#define baud_rate 1600000u
static state_machine_t g_terminal1 = { 0 };
static state_machine_t g_terminal2 = { 0 };

static in_use_flags_t g_flags = { 0 };

/////////////////menu/////////////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tMENU\r" };
static int8_t chat_menu_pos[] = { "\033[12:10H" };
static int8_t chat[] = { "1.	CHAT\r" };
static int8_t set_hour_menu_pos[] = { "\033[13:10H" };
static int8_t set_hour[] = { "2.	SET HOUR\r" };
static int8_t set_date_menu_pos[] = { "\033[14:10H" };
static int8_t set_date[] = { "3.	SET DATE\r" };
static int8_t read_hour_menu_pos[] = { "\033[15:10H" };
static int8_t read_hour[] = { "4.	READ HOUR\r" };
static int8_t display_hour_menu_pos[] = { "\033[16:10H" };
static int8_t disp_hour[] = { "5.	DISPLAY HOUR IN MATRIX\r" };
static int8_t set_msg_menu_pos[] = { "\033[17:10H" };
static int8_t set_msg[] = { "6.	SET MESSAGE\r" };
static int8_t disp_msg_menu_pos[] = { "\033[18:10H" };
static int8_t disp_msg[] = { "7.	DISPLAY MESSAGE IN MATRIX\r" };
static int8_t get_date_menu_pos[] = { "\033[19:10H" };
static int8_t get_date[] = { "8.	READ DATE\r\n" };

static int8_t option_not_available_pos[] = { "\033[08:10H" };
static int8_t option_no_available[] = { "option is being used by the other terminal\r\n please try again later\r\n" };
//static int8_t inst_pos[] = { "\033[010:10H" };
static int8_t inst[] = { "press [ESC] key to continue....\r\n" };

void PSM_main_menu(terminals ter)
{
	switch ((uint8_t) ter)
	{
		case TERMINAL1:
			UART_put_string(UART_0, &clean_screen_all[0]);
			UART_put_string(UART_0, &clean_screen[0]);
			UART_put_string(UART_0, &red_back_yellow[0]);
			//UART_put_string(UART_0, &red_back_yellow[0]);
			UART_put_string(UART_0, &title_pos[0]);
			UART_put_string(UART_0, &title[0]);
			UART_put_string(UART_0, &chat_menu_pos[0]);
			UART_put_string(UART_0, &chat[0]);
			UART_put_string(UART_0, &set_hour_menu_pos[0]);
			UART_put_string(UART_0, &set_hour[0]);
			UART_put_string(UART_0, &set_date_menu_pos[0]);
			UART_put_string(UART_0, &set_date[0]);
			UART_put_string(UART_0, &read_hour_menu_pos[0]);
			UART_put_string(UART_0, &read_hour[0]);
			UART_put_string(UART_0, &display_hour_menu_pos[0]);
			UART_put_string(UART_0, &disp_hour[0]);
			UART_put_string(UART_0, &set_msg_menu_pos[0]);
			UART_put_string(UART_0, &set_msg[0]);
			UART_put_string(UART_0, &disp_msg_menu_pos[0]);
			UART_put_string(UART_0, &disp_msg[0]);
			UART_put_string(UART_0, &get_date_menu_pos[0]);
			UART_put_string(UART_0, &get_date[0]);
		break;
		case TERMINAL2:
			UART_put_string(UART_1, &clean_screen_all[0]);
			UART_put_string(UART_1, &clean_screen[0]);
			UART_put_string(UART_1, &red_back_yellow[0]);
			//UART_put_string(UART_0, &red_back_yellow[0]);
			UART_put_string(UART_1, &title_pos[0]);
			UART_put_string(UART_1, &title[0]);
			UART_put_string(UART_1, &chat_menu_pos[0]);
			UART_put_string(UART_1, &chat[0]);
			UART_put_string(UART_1, &set_hour_menu_pos[0]);
			UART_put_string(UART_1, &set_hour[0]);
			UART_put_string(UART_1, &set_date_menu_pos[0]);
			UART_put_string(UART_1, &set_date[0]);
			UART_put_string(UART_1, &read_hour_menu_pos[0]);
			UART_put_string(UART_1, &read_hour[0]);
			UART_put_string(UART_1, &display_hour_menu_pos[0]);
			UART_put_string(UART_1, &disp_hour[0]);
			UART_put_string(UART_1, &set_msg_menu_pos[0]);
			UART_put_string(UART_1, &set_msg[0]);
			UART_put_string(UART_1, &disp_msg_menu_pos[0]);
			UART_put_string(UART_1, &disp_msg[0]);
			UART_put_string(UART_1, &get_date_menu_pos[0]);
			UART_put_string(UART_1, &get_date[0]);
		break;

	}

}

void PSM_UARTS_setting(void)
{
	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);

	/**Configures the pin control register of pin16 in PortB as UART TX*/
	gpio_pin_control_register_t Rx0_tx0_pcr = GPIO_MUX3;
	/**Configures the pin control register of pin16 in PortB as UART TX*/
	GPIO_pin_control_register(GPIO_B, bit_16, &Rx0_tx0_pcr);
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	GPIO_pin_control_register(GPIO_B, bit_17, &Rx0_tx0_pcr);

	/**Configures the pin control register of pin16 in PortB as UART TX*/
	GPIO_pin_control_register(GPIO_C, bit_3, &Rx0_tx0_pcr);
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	GPIO_pin_control_register(GPIO_C, bit_4, &Rx0_tx0_pcr);

	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init(UART_0, 21000000, BD_115200);
	UART_init(UART_1, 21000000, BD_115200);
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_0);
	UART_interrupt_enable(UART_1);
	/**Enables the UART 0 interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_1);
	NVIC_enable_interrupt_and_priotity(UART1_IRQ, PRIORITY_1);

	NVIC_global_enable_interrupts;
}

uint8_t PSM_GET_CHANGE(terminals state)
{
	switch ((uint8_t) state)
	{
		case TERMINAL1:
			switch (g_terminal1.current_state)
			{
				case ST_CHAT:
					g_terminal1.exit_state = CHAT_get_exit_flag(CHAT_TERMINAL1);
					if (g_terminal1.exit_state) {
						CHAT_clean_exit_flag(MSG_TERMINAL1);
						g_flags.uart0_active_f = FALSE;
						g_terminal1.current_state = ST_MENU;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_SET_HOUR:
					g_terminal1.exit_state = SET_TIME_get_exit_flag(TIME_TERMINAL1);
					if (g_terminal1.exit_state) {
						SET_TIME_clean_exit_flag(TIME_TERMINAL1);
						g_terminal1.current_state = ST_MENU;
						g_flags.in_use_set_time_f = FALSE;
						g_terminal1.change_state = TRUE;

					}
				break;
				case ST_SET_DATE:
					g_terminal1.exit_state = SET_DATE_get_exit_flag(DATE_TERMINAL1);
					if (g_terminal1.exit_state) {
						SET_DATE_clean_exit_flag(DATE_TERMINAL1);
						g_terminal1.current_state = ST_MENU;
						g_flags.in_use_set_date_f = FALSE;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_READ_HOUR:
					g_terminal1.exit_state = GET_TIME_get_exit_flag(GTIME_TERMINAL1);
					if (g_terminal1.exit_state) {
						GET_TIME_clean_exit_flag(GTIME_TERMINAL1);
						g_terminal1.current_state = ST_MENU;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_DISP_HOUR_MAT:
					g_terminal1.exit_state = DISP_TIME_get_exit_flag(DTIME_TERMINAL1);
					if (g_terminal1.exit_state) {
						DISP_TIME_clean_exit_flag(DTIME_TERMINAL1);
						g_flags.in_use_matrix_f = FALSE;
						g_terminal1.current_state = ST_MENU;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_SET_MSG:
					g_terminal1.exit_state = SET_MSG_get_exit_flag(MSG_TERMINAL1);
					if (g_terminal1.exit_state) {
						SET_MSG_clean_exit_flag(MSG_TERMINAL1);
						g_terminal1.current_state = ST_MENU;
						g_flags.in_use_set_msg_f = FALSE;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_DISP_MSG:
					g_terminal1.exit_state = GET_MSG_get_exit_flag(GMSG_TERMINAL1);
					if (g_terminal1.exit_state) {
						GET_MSG_clean_exit_flag(GMSG_TERMINAL1);
						g_terminal1.current_state = ST_MENU;
						g_flags.in_use_matrix_f = FALSE;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_READ_DATE:
					g_terminal1.exit_state = GET_DATE_get_exit_flag(GDATE_TERMINAL1);
					if (g_terminal1.exit_state) {
						GET_DATE_clean_exit_flag(GDATE_TERMINAL1);
						g_terminal1.current_state = ST_MENU;
						g_terminal1.change_state = TRUE;
					}

				break;
				default:

				break;
			}
			return g_terminal1.change_state;
		break;
		case TERMINAL2:
			switch (g_terminal2.current_state)
			{
				case ST_CHAT:
					g_terminal2.exit_state = CHAT_get_exit_flag(CHAT_TERMINAL2);
					if (g_terminal2.exit_state) {
						CHAT_clean_exit_flag(MSG_TERMINAL2);
						g_terminal2.current_state = ST_MENU;
						g_flags.uart1_active_f = FALSE;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_SET_HOUR:
					g_terminal2.exit_state = SET_TIME_get_exit_flag(TIME_TERMINAL2);
					if (g_terminal2.exit_state) {
						SET_TIME_clean_exit_flag(TIME_TERMINAL2);
						g_terminal2.current_state = ST_MENU;
						g_flags.in_use_set_time_f = FALSE;
						g_terminal2.change_state = TRUE;

					}
				break;
				case ST_SET_DATE:
					g_terminal2.exit_state = SET_DATE_get_exit_flag(DATE_TERMINAL2);
					if (g_terminal2.exit_state) {
						SET_DATE_clean_exit_flag(DATE_TERMINAL2);
						g_terminal2.current_state = ST_MENU;
						g_flags.in_use_set_date_f = FALSE;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_READ_HOUR:
					g_terminal2.exit_state = GET_TIME_get_exit_flag(GTIME_TERMINAL2);
					if (g_terminal2.exit_state) {
						GET_TIME_clean_exit_flag(GTIME_TERMINAL2);
						g_terminal2.current_state = ST_MENU;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_DISP_HOUR_MAT:
					g_terminal2.exit_state = DISP_TIME_get_exit_flag(DTIME_TERMINAL2);
					if (g_terminal2.exit_state) {
						DISP_TIME_clean_exit_flag(DTIME_TERMINAL2);
						g_flags.in_use_matrix_f = FALSE;
						g_terminal2.current_state = ST_MENU;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_SET_MSG:
					g_terminal2.exit_state = SET_MSG_get_exit_flag(MSG_TERMINAL2);
					if (g_terminal2.exit_state) {
						SET_MSG_clean_exit_flag(MSG_TERMINAL2);
						g_terminal2.current_state = ST_MENU;
						g_flags.in_use_set_msg_f = FALSE;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_DISP_MSG:
					g_terminal2.exit_state = GET_MSG_get_exit_flag(GMSG_TERMINAL2);
					if (g_terminal2.exit_state) {
						GET_MSG_clean_exit_flag(GMSG_TERMINAL2);
						g_terminal2.current_state = ST_MENU;
						g_flags.in_use_matrix_f = FALSE;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_READ_DATE:
					g_terminal2.exit_state = GET_DATE_get_exit_flag(GDATE_TERMINAL2);
					if (g_terminal2.exit_state) {
						GET_DATE_clean_exit_flag(GDATE_TERMINAL2);
						g_terminal2.current_state = ST_MENU;
						g_terminal2.change_state = TRUE;
					}

				break;
				default:
				break;
			}
			return g_terminal2.change_state;
		break;
		default:
		break;
	}
	return FALSE;
}
void PSM_STM(uint8_t state)
{
	switch (state)
	{
		case TERMINAL1:

			switch (g_terminal1.current_state)
			{
				case ST_MENU:
					PSM_main_menu(TERMINAL1);
					UART_callback_init(PSM0_STATE_MENU, UART_0);
				break;
				case ST_CHAT:
					CHAT_display(CHAT_TERMINAL1);
					UART_callback_init(CHAT_uart0_handler, UART_0);

				break;
				case ST_SET_HOUR:
					if (!g_flags.in_use_set_time_f) {
						g_flags.in_use_set_time_f = TRUE;
						SET_TIME_display(TIME_TERMINAL1);
						UART_callback_init(SET_TIME_uart0_handler, UART_0);
					} else {
						g_terminal1.current_state = ST_BUSY;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_SET_DATE:
					if (!g_flags.in_use_set_date_f) {
						g_flags.in_use_set_date_f = TRUE;
						SET_DATE_display(DATE_TERMINAL1);
						UART_callback_init(SET_DATE_uart0_handler, UART_0);
					} else {
						g_terminal1.current_state = ST_BUSY;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_READ_HOUR:
					GET_TIME_display(TIME_TERMINAL1);
					UART_callback_init(GET_TIME_uart0_handler, UART_0);
				break;
				case ST_DISP_HOUR_MAT:
					if (!g_flags.in_use_matrix_f) {
						g_flags.in_use_matrix_f = TRUE;
						DISP_TIME_display(DTIME_TERMINAL1);
						UART_callback_init(DISP_TIME_uart0_handler, UART_0);
					} else {
						g_terminal1.current_state = ST_BUSY;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_SET_MSG:
					if (!g_flags.in_use_set_msg_f) {
						g_flags.in_use_set_msg_f = TRUE;
						SET_MSG_display(MSG_TERMINAL1);
						UART_callback_init(SET_MSG_uart0_handler, UART_0);
					} else {
						g_terminal1.current_state = ST_BUSY;
						g_terminal1.change_state = TRUE;
					}
				break;
				case ST_DISP_MSG:
					if (!g_flags.in_use_matrix_f) {
						g_flags.in_use_matrix_f = TRUE;
						GET_MSG_display(GMSG_TERMINAL1);
						UART_callback_init(GET_MSG_uart0_handler, UART_0);
					} else {
						g_terminal1.current_state = ST_BUSY;
						g_terminal1.change_state = TRUE;

					}
				break;
				case ST_READ_DATE:
					GET_DATE_display(DATE_TERMINAL1);
					UART_callback_init(GET_DATE_uart0_handler, UART_0);
				break;

				default:
				break;

			}
			if (g_terminal1.current_state == ST_BUSY) {

				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &option_not_available_pos[0]);
				UART_put_string(UART_0, &option_no_available[0]);
				UART_put_string(UART_0, &inst[0]);
				UART_callback_init(PSM_Busy_T1_handler, UART_0);
			}

			PSM_CLEAN_CHANGE(TERMINAL1);
		break;
		case TERMINAL2:
			switch (g_terminal2.current_state)
			{
				case ST_MENU:
					PSM_main_menu(TERMINAL2);
					UART_callback_init(PSM1_STATE_MENU, UART_1);
				break;
				case ST_CHAT:
					CHAT_display(CHAT_TERMINAL2);
					UART_callback_init(CHAT_uart1_handler, UART_1);

				break;
				case ST_SET_HOUR:
					if (!g_flags.in_use_set_time_f) {
						g_flags.in_use_set_time_f = TRUE;
						SET_TIME_display(TIME_TERMINAL2);
						UART_callback_init(SET_TIME_uart1_handler, UART_1);
					} else {
						g_terminal2.current_state = ST_BUSY;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_SET_DATE:
					if (!g_flags.in_use_set_date_f) {
						g_flags.in_use_set_date_f = TRUE;
						SET_DATE_display(DATE_TERMINAL2);
						UART_callback_init(SET_DATE_uart1_handler, UART_1);
					} else {
						g_terminal2.current_state = ST_BUSY;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_READ_HOUR:
					GET_TIME_display(TIME_TERMINAL2);
					UART_callback_init(GET_TIME_uart1_handler, UART_1);
				break;
				case ST_DISP_HOUR_MAT:
					if (!g_flags.in_use_matrix_f) {
						g_flags.in_use_matrix_f = TRUE;
						DISP_TIME_display(DTIME_TERMINAL2);
						UART_callback_init(DISP_TIME_uart1_handler, UART_1);
					} else {
						g_terminal2.current_state = ST_BUSY;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_SET_MSG:
					if (!g_flags.in_use_set_msg_f) {
						g_flags.in_use_set_msg_f = TRUE;
						SET_MSG_display(MSG_TERMINAL2);
						UART_callback_init(SET_MSG_uart1_handler, UART_1);
					} else {
						g_terminal2.current_state = ST_BUSY;
						g_terminal2.change_state = TRUE;
					}
				break;
				case ST_DISP_MSG:
					if (!g_flags.in_use_matrix_f) {
						g_flags.in_use_matrix_f = TRUE;
						GET_MSG_display(GMSG_TERMINAL2);
						UART_callback_init(GET_MSG_uart1_handler, UART_1);
					} else {
						g_terminal2.current_state = ST_BUSY;
						g_terminal2.change_state = TRUE;

					}
				break;
				case ST_READ_DATE:
					GET_DATE_display(DATE_TERMINAL2);
					UART_callback_init(GET_DATE_uart1_handler, UART_1);
				break;

				default:
				break;

			}
			if (g_terminal2.current_state == ST_BUSY) {

				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &option_not_available_pos[0]);
				UART_put_string(UART_1, &option_no_available[0]);
				UART_put_string(UART_1, &inst[0]);
				UART_callback_init(PSM_Busy_T2_handler, UART_1);
			}

			PSM_CLEAN_CHANGE(TERMINAL2);
		break;
		default:
		break;

	}

}

void PSM_CLEAN_CHANGE(terminals state)
{
	switch ((uint8_t) state)
	{
		case TERMINAL1:
			g_terminal1.change_state = FALSE;
		break;
		case TERMINAL2:
			g_terminal2.change_state = FALSE;
		break;
	}
}
void PSM_Busy_T1_handler(void)
{

	g_terminal1.current_state = ST_MENU;
	g_terminal1.change_state = TRUE;

}
void PSM_Busy_T2_handler(void)
{
	g_terminal2.current_state = ST_MENU;
	g_terminal2.change_state = TRUE;
}

void PSM0_STATE_MENU(void)
{
	switch (UART_get_mailbox(UART_0))
	{
		case '1':
			g_terminal1.current_state = ST_CHAT;
		break;
		case '2':
			g_terminal1.current_state = ST_SET_HOUR;
		break;
		case '3':
			g_terminal1.current_state = ST_SET_DATE;
		break;
		case '4':
			g_terminal1.current_state = ST_READ_HOUR;
		break;
		case '5':
			g_terminal1.current_state = ST_DISP_HOUR_MAT;
		break;
		case '6':
			g_terminal1.current_state = ST_SET_MSG;
		break;
		case '7':
			g_terminal1.current_state = ST_DISP_MSG;
		break;
		case '8':
			g_terminal1.current_state = ST_READ_DATE;
		break;
		default:
		break;

	}
	g_terminal1.change_state = TRUE;

}
void PSM1_STATE_MENU(void)
{
	switch (UART_get_mailbox(UART_1))
	{
		case '1':
			g_terminal2.current_state = ST_CHAT;
		break;
		case '2':
			g_terminal2.current_state = ST_SET_HOUR;
		break;
		case '3':
			g_terminal2.current_state = ST_SET_DATE;
		break;
		case '4':
			g_terminal2.current_state = ST_READ_HOUR;
		break;
		case '5':
			g_terminal2.current_state = ST_DISP_HOUR_MAT;
		break;
		case '6':
			g_terminal2.current_state = ST_SET_MSG;
		break;
		case '7':
			g_terminal2.current_state = ST_DISP_MSG;
		break;
		case '8':
			g_terminal2.current_state = ST_READ_DATE;
		break;
		default:
		break;

	}
	g_terminal2.change_state = TRUE;

}

void PSM_INIT(void)
{
	PSM_UARTS_setting();
	I2C_init(I2C_0, CLK_K64, baud_rate);
	HT16k33_init();
	screen_clear_data();
	screen_send_array_2mat();
	M24LC256_fill_memory_With('\0');
	M24LC256_fill_memory_With('\0');

	g_terminal1.current_state = ST_MENU;
	g_terminal2.current_state = ST_MENU;
	g_terminal1.change_state = TRUE;
	g_terminal2.change_state = TRUE;
}

