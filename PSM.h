/*
 * PSM.h
 *
 *  Created on: Nov 17, 2019
 *      Author: alvar
 */

#ifndef PSM_H_
#define PSM_H_

#include <date/set_date.h>
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

#include "time/set_time.h"
#include "time/disp_time.h"
#include "date/set_date.h"

#include "time/get_time.h"
#include "date/get_date.h"


#include "msg/get_msg.h"
#include "msg/set_msg.h"

#include "chat/chat.h"


typedef enum{
	ST_MENU,ST_CHAT,ST_SET_HOUR,ST_SET_DATE,ST_READ_HOUR,ST_DISP_HOUR_MAT,ST_SET_MSG,ST_DISP_MSG,ST_READ_DATE,ST_BUSY
}pms_states_t;

typedef struct {
	 uint8_t current_state;
	 uint8_t change_state;
	 uint8_t exit_state;
}state_machine_t;

typedef enum{
	TERMINAL1,
	TERMINAL2
}terminals;


typedef struct{
		uint8_t in_use_set_time_f;
		uint8_t in_use_set_date_f;
		uint8_t in_use_matrix_f;
		uint8_t uart0_active_f;
		uint8_t uart1_active_f;
		uint8_t in_use_set_msg_f;
	}in_use_flags_t;

void PSM_INIT(void);

uint8_t PSM_GET_CHANGE(terminals state);
void PSM_STM(uint8_t);

void PSM_CLEAN_CHANGE(terminals state);
void PSM_main_menu(terminals ter);

void PSM0_STATE_MENU(void);
void PSM1_STATE_MENU(void);

void PSM_Busy_T1_handler(void);
void PSM_Busy_T2_handler(void);

#endif /* PSM_H_ */
