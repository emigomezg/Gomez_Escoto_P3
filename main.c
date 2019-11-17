/**
 \file
 \brief
 This file contains the main implementation for the K64 UART and Teraterm
 communication.
 \author Emiliano Gomez Guerrero
 \date   18/02/2019
 \todo:
 Interrupts are not implemented in this API implementation.
 */

#include "MK64F12.h"
#include "UART_drivers.h"
#include "NVIC.h"
#include "bits.h"

int main (void)
{
	uint8_t data = 0;
	uint8_t flag = 0;
    UART_init(UART_0, SYSTEM_CLOCK, BD_115200);
    UART_interrupt_enable(UART_0);

    UART_callback_init(UART0_reception_handler, UART_0);

    NVIC_enable_interrupt_and_priotity(UART0_RX_TX_IRQn, PRIORITY_5);
    NVIC_global_enable_interrupts;


    for(;;)
    {
    	flag = get_UART_flag_state(UART_0);
    	if(TRUE == flag)
    	{
    		data = get_UART_data_state(UART_0);
    		flag = 0;
    	}
    }
    return 0;
}
