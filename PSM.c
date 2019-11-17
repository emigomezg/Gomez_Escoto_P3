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


void psm_handler(void);
//static PSM_states_t g_flags = { 0 };

/////////////////menu/////////////////////////////////////
static int8_t clean_screen[] = { "\033[2J" };
static int8_t green_text_red_char_background[] = { "\033[0;32;41m" };
static int8_t red_back_yellow[] = { "\033[0;35;43m" };/** VT100 command for text in red and background in cyan*/
static int8_t title_pos[] = { "\033[10:11H" };
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
static int8_t get_date[] = { "8.	READ DATE\r" };

void PSM_main_menu(void)
{
	UART_put_string(UART_0, &clean_screen[0]);
	UART_put_string(UART_0, &green_text_red_char_background[0]);
	UART_put_string(UART_0, &red_back_yellow[0]);
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

}

void PSM_INIT(void)
{

	PIT_clock_gating(); //enables pit
	PIT_enable();

	NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_0); //sets pit priority
	NVIC_global_enable_interrupts;

	PIT_delay(PIT_1, CLK_K64, DELAYS);
	PIT_enable_timer(PIT_1);
	PIT_callback_init(psm_handler, PIT_1);
	PIT_enable_interrupt(PIT_1);
	//I2C_init(I2C_0, CLK_K64, baud_rate);///global init for I2C at 400kbs
	//HT16k33_init();
	//screen_clear_data();
	//HT16k33_cleanMat();
	/**Enables interrupts*/
	//gpio_pin_control_register_t Rx0_tx0_pcr = GPIO_MUX3;
	NVIC_global_enable_interrupts;
	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	//GPIO_clock_gating(GPIO_B);
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[16] = PORT_PCR_MUX(3);
	/**Configures the pin control register of pin16 in PortB as UART TX*/
	PORTB->PCR[17] = PORT_PCR_MUX(3);

	/**Configures the pin control register of pin16 in PortB as UART TX*/
	//GPIO_pin_control_register(GPIO_B, bit_16, &Rx0_tx0_pcr);
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	//GPIO_pin_control_register(GPIO_B, bit_17, &Rx0_tx0_pcr);
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init(UART_0, 21000000, BD_115200);
	//UART_init(UART_1, 21000000, BD_115200);
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_0);
	//UART_interrupt_enable(UART_1);
	/**Enables the UART 0 interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_10);
	//NVIC_enable_interrupt_and_priotity(UART1_IRQ, PRIORITY_1);
	//NVIC_global_enable_interrupts;
	PSM_main_menu();
	//UART0_init;
	//UART1_init;
//	PSM_MENU(UART0);
//	PSM_MENU(UART1);
}

void psm_handler(void)
{}

