/*
 * set_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef TIME_SET_TIME_H_
#define TIME_SET_TIME_H_

#include "MK64F12.h"
#include "../fifo.h"
#include "../drivers/UART.h"
#include "../MCP7940M_drivers/MCP7940M.h"

#define TIME_FIFO_SIZE 8

typedef enum{
	TIME_TERMINAL0,TIME_TERMINAL1
}time_profil_t;






void SET_TIME_display(time_profil_t terminal);
void SET_TIME_uart0_handler(void);
void SET_TIME_uart1_handler(void);
uint8_t SET_TIME_get_exit_flag(time_profil_t terminal);
void SET_TIME_clean_exit_flag(time_profil_t terminal);



#endif /* TIME_SET_TIME_H_ */
