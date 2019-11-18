/*
 * fifo.h
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#ifndef FIFO_H_
#define FIFO_H_

#include "MK64F12.h"
#define FIFO_MAX_DATA_SIZE 99


typedef struct{
	uint8_t data[FIFO_MAX_DATA_SIZE+1];
	uint8_t status;
	uint8_t index;
	uint8_t max_size;
}fifos_t;

typedef enum{
	FIFO_EMPTY, FIFO_AVALABLE,FIFO_FULL,FIFO_SUCCESS,FIFO_ERR
}fifo_status_t;
typedef enum{
	FIFO_CHAT,FIFO_TIME,FIFO_DATE, FIFO_MSG
}fifo_types_t;


fifo_status_t FIFO_init(fifos_t * fifo,uint8_t size);
uint8_t FIFO_push(fifos_t * fifo,uint8_t data);
uint8_t FIFO_POP(fifos_t * fifo);
uint8_t FIFO_isEmpty(fifos_t * fifo);
uint8_t FIFO_getStatus(fifos_t * fifo);


#endif /* FIFO_H_ */
