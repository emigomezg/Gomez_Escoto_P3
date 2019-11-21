/*
 * disp_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef TIME_DISP_TIME_H_
#define TIME_DISP_TIME_H_
#include <source/drivers/PIT.h>
#include <source/drivers/NVIC.h>
#include <source/fifo.h>
#include <source/drivers/UART.h>
#include <source/MCP7940M_drivers/MCP7940M.h>
#include <source/adafruit/LETTERS_NUMS.h>
#include <source/adafruit/ht16k33.h>
#include <source/adafruit/screen.h>


#define DTIME_FIFO_SIZE 8

typedef enum{
	DTIME_TERMINAL1,DTIME_TERMINAL2
}dtime_profil_t;

void DISP_TIME_display(dtime_profil_t terminal);
void DISP_TIME_uart0_handler(void);
void DISP_TIME_uart1_handler(void);
uint8_t DISP_TIME_get_exit_flag(dtime_profil_t terminal);
void DISP_TIME_clean_exit_flag(dtime_profil_t terminal);
void DISP_TIMER0_PIT_handler(void);
void DISP_TIMER1_PIT_handler(void);

void DISP_TIME_send_to_UART(dtime_profil_t terminal);



#endif /* TIME_DISP_TIME_H_ */
