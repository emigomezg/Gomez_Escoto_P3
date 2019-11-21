/*
 * set_time.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef MSG_GET_MSG_H_
#define MSG_GET_MSG_H_

#include "MK64F12.h"
#include "../fifo.h"
#include "../drivers/UART.h"
#include "../24LC256_drivers/M24LC256.h"
#include "../adafruit/LETTERS_NUMS.h"
#include "../adafruit/ht16k33.h"
#include "../adafruit/screen.h"

#include "../drivers/PIT.h"
#include "../drivers/NVIC.h"


#define GMSG1_MEM 0x00
#define GMSG2_MEM 0x20
#define GMSG3_MEM 0x40
#define GMSG4_MEM 0x60
#define GMSG5_MEM 0x80

#define GMSG_FIFO_SIZE 30

typedef enum{
	GMSG_TERMINAL1,GMSG_TERMINAL2
}gmsg_profil_t;

typedef enum{
	GMSG_1 = '1',GMSG_2,GMSG_3,GMSG_4,GMSG_5
}gnum_msg_t;




void GET_MSG_display(gmsg_profil_t terminal);
void GET_MSG_uart0_handler(void);
void GET_MSG_uart1_handler(void);
uint8_t GET_MSG_get_exit_flag(gmsg_profil_t terminal);
void GET_MSG_clean_exit_flag(gmsg_profil_t terminal);
void DISP_MSG_PIT_handler(void);



#endif /* TIME_SET_TIME_H_ */
