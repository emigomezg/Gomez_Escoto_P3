/*
 * get_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef DATE_GET_TIME_H_
#define DATE_GET_TIME_H_

#include <source/drivers/PIT.h>
#include <source/drivers/NVIC.h>
#include <source/fifo.h>
#include <source/drivers/UART.h>
#include <source/MCP7940M_drivers/MCP7940M.h>

#define DATE_FIFO_SIZE 8

typedef enum{
	GDATE_TERMINAL1,GDATE_TERMINAL2
}gdate_profil_t;

void GET_DATE_display(gdate_profil_t terminal);
void GET_DATE_uart0_handler(void);
void GET_DATE_uart1_handler(void);
uint8_t GET_DATE_get_exit_flag(gdate_profil_t terminal);
void GET_DATE_clean_exit_flag(gdate_profil_t terminal);
void GET_DATE0_PIT_handler(void);
void GET_DATE1_PIT_handler(void);

void GET_DATE_send_to_UART(gdate_profil_t terminal);

#endif /* TIME_GET_TIME_H_ */
