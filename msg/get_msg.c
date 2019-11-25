/*
 * set_time.c
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#include <msg/get_msg.h>

#define DELAYS (0.5F)
#define SYSTEM_CLOCK 21000000U

/////////////////////////////////////////start display info///////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t clean_screen_all[] = { "\033[1J" };
//static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:10H" };
static int8_t title[] = { "\tGET MSG\r" };
static int8_t get_msg_pos[] = { "\033[12:10H" };
static int8_t get_msg[] = { " SELECT MESSAGE TO DISPLAY FROM 1 TO 5:\r\n" };
//static int8_t return_status_pos[] = { "\033[18:10H" };
//static int8_t return_status[] = { "message change was:\t" };
//static int8_t unsucc[] = { "Unsuccessful\n\r" };
//static int8_t succ[] = { "successful\n\r" };
static int8_t press_any_key[] = { "press any key to continue....\n\r" };
static int8_t selected[] = { "[SELECTED]\n\r" };

static volatile uint8_t get_msg0_exit_flag = FALSE;
static volatile uint8_t get_msg1_exit_flag = FALSE;

static volatile uint8_t gmsg_select1_f = FALSE;
static volatile uint8_t gmsg_select2_f = FALSE;

static fifos_t gmsg1, gmsg2, gmsg3, gmsg4, gmsg5;

static uint8_t g_limit_reached = FALSE;

void GET_MSG_display(gmsg_profil_t terminal)
{
	g_limit_reached = FALSE;
	switch ((uint8_t) terminal)
	{
		case GMSG_TERMINAL1:
			if (FIFO_init(&gmsg1, GMSG_FIFO_SIZE) == FIFO_SUCCESS) {
				FIFO_init(&gmsg2, GMSG_FIFO_SIZE);
				FIFO_init(&gmsg3, GMSG_FIFO_SIZE);
				FIFO_init(&gmsg4, GMSG_FIFO_SIZE);
				FIFO_init(&gmsg5, GMSG_FIFO_SIZE);
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				UART_put_string(UART_0, &title_pos[0]);
				UART_put_string(UART_0, &title[0]);
				UART_put_string(UART_0, &get_msg_pos[0]);
				UART_put_string(UART_0, &get_msg[0]);

				PIT_clock_gating(); //enables pit
				HT16k33_init();
				PIT_enable();

				NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_3); //sets pit priority
				NVIC_global_enable_interrupts;

				PIT_delay(PIT_1, SYSTEM_CLOCK, DELAYS);
				PIT_enable_timer(PIT_1);
				PIT_callback_init(DISP_MSG_PIT_handler, PIT_1);
				PIT_enable_interrupt(PIT_1);
				PIT_disable_timer(PIT_1);

				get_msg0_exit_flag = FALSE;
				gmsg_select1_f = FALSE;
				uint8_t data;
				//extracts msg 1 from mem
				uint8_t displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG1_MEM + displacement);
					FIFO_push(&gmsg1, data);
					displacement++;
				} while (data != '\0');
				//extracts msg 2 from mem
				data = 0x00;
				displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG2_MEM + displacement);
					FIFO_push(&gmsg2, data);
					displacement++;
				} while (data != '\0');
				//extracts msg 3 from mem
				data = 0x00;
				displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG3_MEM + displacement);
					FIFO_push(&gmsg3, data);
					displacement++;
				} while (data != '\0');
				//extracts msg 4 from mem
				data = 0x00;
				displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG4_MEM + displacement);
					FIFO_push(&gmsg4, data);
					displacement++;
				} while (data != '\0');
				//extracts msg 5 from mem
				data = 0x00;
				displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG5_MEM + displacement);
					FIFO_push(&gmsg5, data);
					displacement++;
				} while (data != '\0');
				UART_put_char(UART_0, '1');
				UART_put_char(UART_0, '.');
				UART_put_char(UART_0, '\t');
				for(uint8_t a = 0; a < gmsg1.index; a++)
					UART_put_char(UART_0, gmsg1.data[a]);
				UART_put_char(UART_0, '\r');
				UART_put_char(UART_0, '\n');

				UART_put_char(UART_0, '2');
				UART_put_char(UART_0, '.');
				UART_put_char(UART_0, '\t');
				for(uint8_t a = 0; a < gmsg2.index; a++)
					UART_put_char(UART_0, gmsg2.data[a]);
				UART_put_char(UART_0, '\r');
				UART_put_char(UART_0, '\n');

				UART_put_char(UART_0, '3');
				UART_put_char(UART_0, '.');
				UART_put_char(UART_0, '\t');
				for(uint8_t a = 0; a < gmsg3.index; a++)
					UART_put_char(UART_0, gmsg3.data[a]);
				UART_put_char(UART_0, '\r');
				UART_put_char(UART_0, '\n');

				UART_put_char(UART_0, '4');
				UART_put_char(UART_0, '.');
				UART_put_char(UART_0, '\t');
				for(uint8_t a = 0; a < gmsg4.index; a++)
					UART_put_char(UART_0, gmsg4.data[a]);
				UART_put_char(UART_0, '\r');
				UART_put_char(UART_0, '\n');

				UART_put_char(UART_0, '5');
				UART_put_char(UART_0, '.');
				UART_put_char(UART_0, '\t');
				for(uint8_t a = 0; a < gmsg5.index; a++)
					UART_put_char(UART_0, gmsg5.data[a]);
				UART_put_char(UART_0, '\r');
				UART_put_char(UART_0, '\n');

			} else {
				UART_put_string(UART_0, &clean_screen_all[0]);
				UART_put_string(UART_0, &clean_screen[0]);
				UART_put_string(UART_0, &red_back_yellow[0]);
				get_msg0_exit_flag = FALSE;
				gmsg_select1_f = FALSE;
			}
		break;
		case GMSG_TERMINAL2:
			if (FIFO_init(&gmsg1, GMSG_FIFO_SIZE) == FIFO_SUCCESS) {
				FIFO_init(&gmsg2, GMSG_FIFO_SIZE);
				FIFO_init(&gmsg3, GMSG_FIFO_SIZE);
				FIFO_init(&gmsg4, GMSG_FIFO_SIZE);
				FIFO_init(&gmsg5, GMSG_FIFO_SIZE);
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				UART_put_string(UART_1, &title_pos[0]);
				UART_put_string(UART_1, &title[0]);
				UART_put_string(UART_1, &get_msg_pos[0]);
				UART_put_string(UART_1, &get_msg[0]);

				PIT_clock_gating(); //enables pit
				HT16k33_init();
				PIT_enable();

				NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_3); //sets pit priority
				NVIC_global_enable_interrupts;

				PIT_delay(PIT_2, SYSTEM_CLOCK, DELAYS);
				PIT_enable_timer(PIT_2);
				PIT_callback_init(DISP_MSG_PIT_handler, PIT_2);
				PIT_enable_interrupt(PIT_2);
				PIT_disable_timer(PIT_2);

				get_msg1_exit_flag = FALSE;
				gmsg_select2_f = FALSE;
				uint8_t data;
				//extracts msg 1 from mem
				uint8_t displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG1_MEM + displacement);
					FIFO_push(&gmsg1, data);
					displacement++;
				} while (data != '\0');
				//extracts msg 2 from mem
				data = 0x00;
				displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG2_MEM + displacement);
					FIFO_push(&gmsg2, data);
					displacement++;
				} while (data != '\0');
				//extracts msg 3 from mem
				data = 0x00;
				displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG3_MEM + displacement);
					FIFO_push(&gmsg3, data);
					displacement++;
				} while (data != '\0');
				//extracts msg 4 from mem
				data = 0x00;
				displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG4_MEM + displacement);
					FIFO_push(&gmsg4, data);
					displacement++;
				} while (data != '\0');
				//extracts msg 5 from mem
				data = 0x00;
				displacement = 0x00;
				do {
					data = M24LC256_read_random(GMSG5_MEM + displacement);
					FIFO_push(&gmsg5, data);
					displacement++;
				} while (data != '\0');
				UART_put_char(UART_1, '1');
				UART_put_char(UART_1, '.');
				UART_put_char(UART_1, '\t');
				for(uint8_t a = 0; a < gmsg1.index; a++)
					UART_put_char(UART_1, gmsg1.data[a]);
				UART_put_char(UART_1, '\r');
				UART_put_char(UART_1, '\n');

				UART_put_char(UART_1, '2');
				UART_put_char(UART_1, '.');
				UART_put_char(UART_1, '\t');
				for(uint8_t a = 0; a < gmsg2.index; a++)
					UART_put_char(UART_1, gmsg2.data[a]);
				UART_put_char(UART_1, '\r');
				UART_put_char(UART_1, '\n');

				UART_put_char(UART_1, '3');
				UART_put_char(UART_1, '.');
				UART_put_char(UART_1, '\t');
				for(uint8_t a = 0; a < gmsg3.index; a++)
					UART_put_char(UART_1, gmsg3.data[a]);
				UART_put_char(UART_1, '\r');
				UART_put_char(UART_1, '\n');

				UART_put_char(UART_1, '4');
				UART_put_char(UART_1, '.');
				UART_put_char(UART_1, '\t');
				for(uint8_t a = 0; a < gmsg4.index; a++)
					UART_put_char(UART_1, gmsg4.data[a]);
				UART_put_char(UART_1, '\r');
				UART_put_char(UART_1, '\n');

				UART_put_char(UART_1, '5');
				UART_put_char(UART_1, '.');
				UART_put_char(UART_1, '\t');
				for(uint8_t a = 0; a < gmsg5.index; a++)
					UART_put_char(UART_1, gmsg5.data[a]);
				UART_put_char(UART_1, '\r');
				UART_put_char(UART_1, '\n');

			} else {
				UART_put_string(UART_1, &clean_screen_all[0]);
				UART_put_string(UART_1, &clean_screen[0]);
				UART_put_string(UART_1, &red_back_yellow[0]);
				get_msg1_exit_flag = FALSE;
				gmsg_select2_f = FALSE;
			}
		break;
		default:
		break;

	}

}

void GET_MSG_uart0_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
	static volatile uint8_t msg_2_change = 0x00;
	uint8_t data_recived = UART_get_mailbox(UART_0);
	if (wait_flag == TRUE) {
		get_msg0_exit_flag = TRUE;
		wait_flag = FALSE;
		PIT_disable_timer(PIT_1);
		screen_clear_data();
		screen_send_array_2mat();
	} else if (data_recived != '\e' && data_recived != '\r') {
		if (!gmsg_select1_f) {
			if (data_recived > '0' && data_recived <= '5') {
				UART_put_char(UART_0, data_recived);
				UART_put_string(UART_0, &selected[0]);
				UART_put_char(UART_0, '\r');
				UART_put_char(UART_0, '\n');
				UART_put_string(UART_0, &press_any_key[0]);
				msg_2_change = data_recived;
				gmsg_select1_f = TRUE;
				wait_flag = TRUE;
			}
			switch (msg_2_change)
			{
				case GMSG_1:
					screen_clear_data();
					while (gmsg1.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg1));
					}
					PIT_enable_timer(PIT_1);

				break;
				case GMSG_2:
					screen_clear_data();
					while (gmsg2.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg2));
					}
					PIT_enable_timer(PIT_1);
				break;
				case GMSG_3:
					screen_clear_data();
					while (gmsg3.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg3));
					}
					PIT_enable_timer(PIT_1);
				break;
				case GMSG_4:
					screen_clear_data();
					while (gmsg4.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg4));
					}
					PIT_enable_timer(PIT_1);
				break;
				case GMSG_5:
					screen_clear_data();
					while (gmsg5.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg5));
					}
					PIT_enable_timer(PIT_1);
				break;
				default:
				break;

			}
		}

	} else if (data_recived == '\e') {
		get_msg0_exit_flag = TRUE;
	}

}
void GET_MSG_uart1_handler(void)
{
	static volatile uint8_t wait_flag = FALSE;
	static volatile uint8_t msg_2_change = 0x00;
	uint8_t data_recived = UART_get_mailbox(UART_1);
	if (wait_flag == TRUE) {
		get_msg1_exit_flag = TRUE;
		wait_flag = FALSE;
		PIT_disable_timer(PIT_2);
		screen_clear_data();
		screen_send_array_2mat();
	} else if (data_recived != '\e' && data_recived != '\r') {
		if (!gmsg_select2_f) {
			if (data_recived > '0' && data_recived <= '5') {
				UART_put_char(UART_1, data_recived);
				UART_put_string(UART_1, &selected[0]);
				UART_put_char(UART_1, '\r');
				UART_put_char(UART_1, '\n');
				UART_put_string(UART_1, &press_any_key[0]);
				msg_2_change = data_recived;
				gmsg_select2_f = TRUE;
				wait_flag = TRUE;
			}
			switch (msg_2_change)
			{
				case GMSG_1:
					screen_clear_data();
					while (gmsg1.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg1));
					}
					PIT_enable_timer(PIT_2);

				break;
				case GMSG_2:
					screen_clear_data();
					while (gmsg2.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg2));
					}
					PIT_enable_timer(PIT_2);
				break;
				case GMSG_3:
					screen_clear_data();
					while (gmsg3.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg3));
					}
					PIT_enable_timer(PIT_2);
				break;
				case GMSG_4:
					screen_clear_data();
					while (gmsg4.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg4));
					}
					PIT_enable_timer(PIT_2);
				break;
				case GMSG_5:
					screen_clear_data();
					while (gmsg5.index != 1) {
						screen_add_Item_end(FIFO_POP(&gmsg5));
					}
					PIT_enable_timer(PIT_2);
				break;
				default:
				break;

			}
		}

	} else if (data_recived == '\e') {
		get_msg1_exit_flag = TRUE;
	}

}
void DISP_MSG_PIT_handler(void)
{
	screen_send_array_2mat();
	screen_slideR();
}

uint8_t GET_MSG_get_exit_flag(gmsg_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case GMSG_TERMINAL1:
			return get_msg0_exit_flag;
		break;
		case GMSG_TERMINAL2:
			return get_msg1_exit_flag;
		break;
		default:
			return 0x00;
		break;
	}
}
void GET_MSG_clean_exit_flag(gmsg_profil_t terminal)
{
	switch ((uint8_t) terminal)
	{
		case GMSG_TERMINAL1:
			get_msg0_exit_flag = FALSE;
		break;
		case GMSG_TERMINAL2:
			get_msg1_exit_flag = FALSE;
		break;
		default:
		break;
	}
}
