/*
 * M24LC256.c
 *
 *  Created on: Nov 12, 2019
 *      Author: alvaro
 */

#include "M24LC256.h"
#define M24LC256_READ 0xA1
#define M24LC256_WRITE 0xA0

static const uint8_t g_base_address = 0x00;
static uint8_t g_address = 0x00;

void M24LC256_set_i2c_mem(a0_m24lc256_t a0, a0_m24lc256_t a1, a2_m24lc256_t a2)
{
	g_address = g_base_address | (a0 << A0_SHIFT) | (a1 << A1_SHIFT) | (a2 << A2_SHIFT);
}

uint8_t M24LC256_Write_random(uint8_t data, uint16_t str_add)
{
	uint8_t pass = FALSE;
	uint8_t high_address = (uint8_t) (str_add & 0x7F00);
	uint8_t low_addres = (uint8_t) str_add & 0xFF;

	I2C_start(I2C_0); /*sends start bit*/
	I2C_write_byte(I2C_0, g_address + M24LC256_WRITE); /*writes the direction of RTC with the write bit enable*/
	I2C_wait(I2C_0); /*waits for response in the line*/
	pass = !(I2C_get_ack(I2C_0)); /*reads the acknowledge*/
	if(pass){
	I2C_write_byte(I2C_0, high_address);/*HIGH half of memory address*/
	I2C_wait(I2C_0); /*waits for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	I2C_write_byte(I2C_0, low_addres);/*HIGH half of memory address*/
	I2C_wait(I2C_0); /*waits for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	I2C_write_byte(I2C_0, data); /*set data in address*/
	I2C_wait(I2C_0); /*wait for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/
	I2C_stop(I2C_0); /*sends nack and stop communications*/
	return TRUE;
	}else
		I2C_stop(I2C_0); /*sends nack and stop communications*/
	return FALSE;

}

void M24LC256_write_sequential(uint8_t *data_array, uint16_t num_data_write, uint16_t str_add)
{
	uint8_t high_address = (uint8_t) (str_add & 0x7F00);
	uint8_t low_addres = (uint8_t) str_add & 0xFF;

	I2C_start(I2C_0); /*sends start bit*/
	I2C_write_byte(I2C_0, g_address + M24LC256_WRITE); /*writes the direction of RTC with the write bit enable*/
	I2C_wait(I2C_0); /*waits for response in the line*/
	I2C_get_ack(I2C_0); /*reads the acknowledge*/

	I2C_write_byte(I2C_0, high_address);/*HIGH half of memory address*/
	I2C_wait(I2C_0); /*waits for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	I2C_write_byte(I2C_0, low_addres);/*HIGH half of memory address*/
	I2C_wait(I2C_0); /*waits for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	for(int a = 0; a < num_data_write; a++) {
		I2C_write_byte(I2C_0, *(data_array + a)); /*set data in address*/
		I2C_wait(I2C_0); /*wait for acknowledge*/
		I2C_get_ack(I2C_0); /*reads acknowledge*/
	}
	I2C_stop(I2C_0); /*sends nack and stop communications*/
}

uint8_t M24LC256_read_random( uint16_t str_add)
{
	uint8_t data;
	uint8_t high_address = (uint8_t) (str_add & 0x7F00);
	uint8_t low_addres = (uint8_t) str_add & 0xFF;

	I2C_start(I2C_0); /*sends start bit*/
	I2C_write_byte(I2C_0, g_address + M24LC256_WRITE); /*writes the direction of RTC with the write bit enable*/
	I2C_wait(I2C_0); /*waits for response in the line*/
	I2C_get_ack(I2C_0); /*reads the acknowledge*/

	I2C_write_byte(I2C_0, high_address);/*HIGH half of memory address*/
	I2C_wait(I2C_0); /*waits for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	I2C_write_byte(I2C_0, low_addres);/*HIGH half of memory address*/
	I2C_wait(I2C_0); /*waits for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	I2C_repeted_start(I2C_0); // sends re-start bit
	I2C_write_byte(I2C_0, g_address + M24LC256_READ); //sends RTC address + read bit
	I2C_wait(I2C_0); //waits for response
	I2C_get_ack(I2C_0); //read acknowledge

	I2C_tx_rx_mode(I2C_0, I2C_RECIVER); //change to receiver mode

	I2C_nack(I2C_0); //send NACK signal
	I2C_read_byte(I2C_0); //reads dummy value
	I2C_wait(I2C_0); //wait to line clear

	I2C_stop(I2C_0); //sends stop bit
	data = I2C_read_byte(I2C_0); //read real value
	return data;
}
void M24LC256_fill_memory_With(uint8_t character){
	uint32_t address = 0x00;
	for(uint32_t a = 0; a<32;a++){

		 uint32_t add_high = ((address >>8) & 0xFF);
		 uint32_t add_low = (address & 0xFF);
		address+=0x40;

		I2C_start(I2C_0); /*sends start bit*/
		I2C_write_byte(I2C_0, g_address + M24LC256_WRITE); /*writes the direction of RTC with the write bit enable*/
		I2C_wait(I2C_0); /*waits for response in the line*/
		I2C_get_ack(I2C_0); /*reads the acknowledge*/

		I2C_write_byte(I2C_0, add_high);/*HIGH half of memory address*/
		I2C_wait(I2C_0); /*waits for acknowledge*/
		I2C_get_ack(I2C_0); /*reads acknowledge*/

		I2C_write_byte(I2C_0, add_low);/*HIGH half of memory address*/
		I2C_wait(I2C_0); /*waits for acknowledge*/
		I2C_get_ack(I2C_0); /*reads acknowledge*/

		for(uint8_t b = 0; b < 64; b++) {
			I2C_write_byte(I2C_0, character); /*set data in address*/
			I2C_wait(I2C_0); /*wait for acknowledge*/
			I2C_get_ack(I2C_0); /*reads acknowledge*/
		}
		I2C_stop(I2C_0); /*sends nack and stop communications*/
	}
}

void M24LC256_read_sequential(uint8_t *data_array, uint16_t num_data_ret, uint16_t str_add)
{
	uint8_t high_address = (uint8_t) (str_add & 0x7F00);
	uint8_t low_addres = (uint8_t) str_add & 0xFF;

	I2C_start(I2C_0); /*sends start bit*/
	I2C_write_byte(I2C_0, g_address + M24LC256_WRITE); /*writes the direction of RTC with the write bit enable*/
	I2C_wait(I2C_0); /*waits for response in the line*/
	I2C_get_ack(I2C_0); /*reads the acknowledge*/

	I2C_write_byte(I2C_0, high_address);/*HIGH half of memory address*/
	I2C_wait(I2C_0); /*waits for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	I2C_write_byte(I2C_0, low_addres);/*HIGH half of memory address*/
	I2C_wait(I2C_0); /*waits for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	I2C_repeted_start(I2C_0); // sends re-start bit
	I2C_write_byte(I2C_0, g_address + M24LC256_READ); //sends RTC address + read bit
	I2C_wait(I2C_0); //waits for response
	I2C_get_ack(I2C_0); //read acknowledge

	I2C_tx_rx_mode(I2C_0, I2C_RECIVER); //change to receiver mode
	for(int a =0; a< num_data_ret;a++)
	{
		I2C_read_byte(I2C_0); //reads dummy value
		I2C_ack(I2C_0);
		I2C_wait(I2C_0); //wait to line clear
		*(data_array+a) = I2C_read_byte(I2C_0); //read real value
	}
	I2C_nack(I2C_0); //send NACK signal
		I2C_read_byte(I2C_0); //reads dummy value
		I2C_wait(I2C_0); //wait to line clear

		I2C_stop(I2C_0); //sends stop bit

}
