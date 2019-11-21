/*
 * set_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef MSG_SET_MSG_H_
#define MSG_SET_MSG_H_

#include "MK64F12.h"
#include "../fifo.h"
#include "../drivers/UART.h"
#include "../24LC256_drivers/M24LC256.h"

#define MSG1_MEM 0x00
#define MSG2_MEM 0x20
#define MSG3_MEM 0x40
#define MSG4_MEM 0x60
#define MSG5_MEM 0x80

#define MSG_FIFO_SIZE 30

typedef enum{
	MSG_TERMINAL1,MSG_TERMINAL2
}msg_profil_t;

typedef enum{
	MSG_1 = '1',MSG_2,MSG_3,MSG_4,MSG_5
}num_msg_t;




void SET_MSG_display(msg_profil_t terminal);
void SET_MSG_uart0_handler(void);
void SET_MSG_uart1_handler(void);
uint8_t SET_MSG_get_exit_flag(msg_profil_t terminal);
void SET_MSG_clean_exit_flag(msg_profil_t terminal);



#endif /* TIME_SET_TIME_H_ */
