/*
 * ht16k33.c
 *
 *  Created on: Nov 13, 2019
 *      Author: alvar
 */

#include "ht16k33.h"
#define CLK_K64 21000000u
#define baud_rate 1600000u

void HT16k33_init(void)
{
	//I2C_init(I2C_0, CLK_K64, baud_rate);
	I2C_start(I2C_0);
	I2C_write_byte(I2C_0, HT16K33_address);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0, 0x21);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_stop(I2C_0);

	I2C_start(I2C_0);
	I2C_write_byte(I2C_0, HT16K33_address);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0, 0x81);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_stop(I2C_0);

	I2C_start(I2C_0);
	I2C_write_byte(I2C_0, HT16K33_address);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0, 0xEF);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_stop(I2C_0);


}

void HT16k33_sendMat( volatile uint8_t *data)
{
	//HT16k33_cleanMat();

		I2C_start(I2C_0);
		I2C_write_byte(I2C_0, HT16K33_address);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_write_byte(I2C_0, HT16K33_START_ADD);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		for(int a = 0;a<16; a++)
		{
			uint8_t temp = *(data+a);
			I2C_write_byte(I2C_0, temp);
			I2C_wait(I2C_0);
			I2C_get_ack(I2C_0);
		}

		I2C_stop(I2C_0);

}
void HT16k33_cleanMat(void)
{
		I2C_start(I2C_0);
		I2C_write_byte(I2C_0, HT16K33_address);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_write_byte(I2C_0, HT16K33_START_ADD);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		for(int a = 0;a<17; a++)
		{
			//uint8_t temp = *(data+a);
			I2C_write_byte(I2C_0, 0x00);
			I2C_wait(I2C_0);
			I2C_get_ack(I2C_0);
		}

		I2C_stop(I2C_0);
}
