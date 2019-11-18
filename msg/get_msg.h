/*
 * get_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef MSG_GET_MSG_H_
#define MSG_GET_MSG_H_

#include <source/drivers/PIT.h>
#include <source/drivers/NVIC.h>
#include <source/fifo.h>
#include <source/drivers/UART.h>
#include <source/MCP7940M_drivers/MCP7940M.h>

#define GMSG_FIFO_SIZE 8

typedef enum{
	GMSG_TERMINAL0,GMSG_TERMINAL1
}gmsg_profil_t;

void GET_MSG_display(gmsg_profil_t terminal);
void GET_MSG_uart0_handler(void);
void GET_MSG_uart1_handler(void);
uint8_t GET_MSG_get_exit_flag(gmsg_profil_t terminal);
void GET_MSG_clean_exit_flag(gmsg_profil_t terminal);
void GET_MSG0_PIT_handler(void);
void GET_MSG1_PIT_handler(void);

void GET_MSG_send_to_UART(gmsg_profil_t terminal);

#endif /* TIME_GET_TIME_H_ */
