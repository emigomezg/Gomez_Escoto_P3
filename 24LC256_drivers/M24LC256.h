/*
 * 24LC256.h
 *
 *  Created on: Nov 12, 2019
 *      Author: alvar
 */

#ifndef M24LC256_DRIVERS_24LC256_H_
#define M24LC256_DRIVERS_24LC256_H_

#define M24LC256_FIRST_ADDRESS 0x0000
#define M24LC256_LAST_ADDRESS 0x7FFF

#include "../drivers/I2C.h"
#include "MK64F12.h"
#include "../drivers/bits.h"

#define A0_SHIFT 0
#define A1_SHIFT 1
#define A2_SHIFT 2

typedef enum{
	A0_OFF,
	A0_ON
}a0_m24lc256_t;
typedef enum{
	A1_OFF,
	A1_ON
}a1_m24lc256_t;
typedef enum{
	A2_OFF,
	A2_ON
}a2_m24lc256_t;

 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it writes one data to one address
  	 \param[in] uint8_t data, DATA = DATA TO WRITE IN MEMORY
  	 \param[in] uint16_t address, address from M24LC256 to store DATA
  	 \return void

  */
uint8_t M24LC256_Write_random(uint8_t data, uint16_t str_add);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	it writes multiple data to multiple addresses
 	 \param[in] uint8_t data, DATA = DATA TO WRITE IN MEMORY
 	 \param[in] uint8_t num_data_write, number of elements to write
  	 \param[in] uint16_t address, start address of 24LC256
  	 \return void

 */
void M24LC256_write_sequential(uint8_t *data_array,uint16_t num_data_write, uint16_t str_add);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 it reads one data from one address
 	 \param[in] uint8_t data, stores read byte into DATA
 	 \param[in] uint16_t address, address from M24LC256 to extract byte
 	 \return void

 */
uint8_t M24LC256_read_random( uint16_t str_add);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	it reads multiple data from multiple addresses
 	 \param[in] uint8_t data, DATA = DATA TO WRITE IN MEMORY
 	 \param[in] uint8_t num_data_write, number of elements to write
  	 \param[in] uint16_t address, start address of 24LC256
  	 \return void

 */
void M24LC256_read_sequential(uint8_t *data_array, uint16_t num_data_ret, uint16_t str_add);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	it fills all memory with a set value
 	 \param[in] uint8_t character
  	 \return void

 */
void M24LC256_fill_memory_With(uint8_t character);




#endif /* 24LC256_DRIVERS_24LC256_H_ */
