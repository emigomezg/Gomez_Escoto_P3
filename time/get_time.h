/*
 * get_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef TIME_GET_TIME_H_
#define TIME_GET_TIME_H_

#include <source/drivers/PIT.h>
#include <source/drivers/NVIC.h>
#include <source/fifo.h>
#include <source/drivers/UART.h>
#include <source/MCP7940M_drivers/MCP7940M.h>

#define TIME_FIFO_SIZE 8

typedef enum{
	GTIME_TERMINAL0,GTIME_TERMINAL1
}gtime_profil_t;

void GET_TIME_display(gtime_profil_t terminal);
void GET_TIME_uart0_handler(void);
void GET_TIME_uart1_handler(void);
uint8_t GET_TIME_get_exit_flag(gtime_profil_t terminal);
void GET_TIME_clean_exit_flag(gtime_profil_t terminal);
void GET_TIMER0_PIT_handler(void);
void GET_TIMER1_PIT_handler(void);

void send_to_UART(gtime_profil_t terminal);

#endif /* TIME_GET_TIME_H_ */
