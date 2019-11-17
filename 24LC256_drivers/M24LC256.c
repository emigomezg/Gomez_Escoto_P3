/*
 * M24LC256.c
 *
 *  Created on: Nov 12, 2019
 *      Author: alvaro
 */

#include "M24LC256.h"
#define M24LC256_READ 1
#define M24LC256_WRITE 0

static const uint8_t g_base_address = 0xA0;
static uint8_t g_address = 0x00;

void M24LC256_set_i2c_mem(a0_m24lc256_t a0, a0_m24lc256_t a1, a2_m24lc256_t a2)
{
	g_address = g_base_address | (a0 << A0_SHIFT) | (a1 << A1_SHIFT) | (a2 << A2_SHIFT);
}

void M24LC256_Write_random(uint8_t data, uint16_t str_add)
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

	I2C_write_byte(I2C_0, data); /*set data in address*/
	I2C_wait(I2C_0); /*wait for acknowledge*/
	I2C_get_ack(I2C_0); /*reads acknowledge*/

	I2C_stop(I2C_0); /*sends nack and stop communications*/
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

void M24LC256_read_random(uint8_t *data, uint16_t str_add)
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

	I2C_nack(I2C_0); //send NACK signal
	I2C_read_byte(I2C_0); //reads dummy value
	I2C_wait(I2C_0); //wait to line clear

	I2C_stop(I2C_0); //sends stop bit
	*data = I2C_read_byte(I2C_0); //read real value
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
