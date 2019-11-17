/*
 * PSM.h
 *
 *  Created on: Nov 17, 2019
 *      Author: alvar
 */

#ifndef PSM_H_
#define PSM_H_

#include "MK64F12.h"
#include "adafruit/ht16k33.h"
#include "adafruit/screen.h"
#include "adafruit/LETTERS_NUMS.h"
#include "drivers/UART.h"
#include "drivers/PIT.h"
#include "drivers/NVIC.h"
#include "drivers/I2C.h"
#include "drivers/bits.h"
#include "MCP7940M_drivers/MCP7940M.h"
#include "drivers/GPIO.h"



typedef struct{
		uint8_t in_use_set_time_f:1;
		uint8_t in_use_set_date_f:1;
		uint8_t in_use_matrix_f:1;
		uint8_t uart0_active_f:1;
		uint8_t uart1_active_f:1;
		uint8_t in_use_set_msg_f:1;
	}in_use_t;
typedef union{
	in_use_t gloabal_flags;
}PSM_states_t;



void PSM_INIT(void);


#endif /* PSM_H_ */
