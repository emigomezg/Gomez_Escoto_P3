/*
 * set_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef DATE_SET_DATE_H_
#define DATE_SET_DATE_H_

#include "MK64F12.h"
#include "../fifo.h"
#include "../drivers/UART.h"
#include "../MCP7940M_drivers/MCP7940M.h"

#define DATE_FIFO_SIZE 8

typedef enum{
	DATE_TERMINAL0,DATE_TERMINAL1
}date_profil_t;






void SET_DATE_display(date_profil_t terminal);
void SET_DATE_uart0_handler(void);
void SET_DATE_uart1_handler(void);
uint8_t SET_DATE_get_exit_flag(date_profil_t terminal);
void SET_DATE_clean_exit_flag(date_profil_t terminal);



#endif /* TIME_SET_TIME_H_ */
