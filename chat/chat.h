/*
 * set_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef CHAT_CHAT_H_
#define CHAT_CHAT_H_

#include "MK64F12.h"
#include "../fifo.h"
#include "../drivers/UART.h"
#include "../drivers/PIT.h"
#include "../drivers/NVIC.h"



#define CHAT_FIFO_SIZE 255

typedef enum{
	CHAT_TERMINAL1,CHAT_TERMINAL2
}chat_profil_t;

typedef enum{
	CHAT_MSG1_TX,CHAT_MSG1_RX,CHAT_MSG2_TX,CHAT_MSG2_RX
}chat_msg_t;




void CHAT_display(chat_profil_t terminal);
void CHAT_uart0_handler(void);
void CHAT_uart1_handler(void);
uint8_t CHAT_get_exit_flag(chat_profil_t terminal);
void CHAT_clean_exit_flag(chat_profil_t terminal);
void CHAT1_PIT_handler(void);


#endif /* TIME_SET_TIME_H_ */
