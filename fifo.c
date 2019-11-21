/*
 * fifo.c
 *
 *  Created on: Nov 18, 2019
 *      Author: alvar
 */

#include "MK64F12.h"
#include "fifo.h"


fifo_status_t FIFO_init(volatile fifos_t * fifo,uint8_t size){

	if(size>0&&size<=FIFO_MAX_DATA_SIZE){
	fifo->max_size = size-1;
	fifo->index = 0;
	fifo->status = FIFO_EMPTY;
	for(uint8_t a = 0;a<FIFO_MAX_DATA_SIZE;a++)
		fifo->data[a]= '\0';
	return FIFO_SUCCESS;
	}
	return FIFO_ERR;

}
fifo_status_t FIFO_push(volatile fifos_t * fifo,uint8_t data)
{
	if(FIFO_getStatus(fifo)!=FIFO_FULL){
		fifo->data[fifo->index] = data;
		fifo->index++;
		if(fifo->index == fifo->max_size+1)
			fifo->status = FIFO_FULL;
		else
			fifo->status = FIFO_AVALABLE;

	}
	return FIFO_getStatus(fifo);
}
uint8_t FIFO_POP(volatile fifos_t * fifo)
{
	uint8_t ret_data  = 0x00;
	if(FIFO_getStatus(fifo)!=FIFO_EMPTY){
		ret_data = fifo->data[0];
		for(uint8_t counter = 0; counter<(fifo->max_size);counter++)
			fifo->data[counter] = fifo->data[counter+1];
		fifo->index--;
		if(fifo->index == FIFO_EMPTY)
			fifo->status = FIFO_EMPTY;
		else
			fifo->status = FIFO_AVALABLE;
	}
	return ret_data;


}
uint8_t FIFO_getStatus(volatile fifos_t * fifo){
	return fifo->status;
}
