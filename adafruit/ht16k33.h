/*
 * ht16k33.h
 *
 *  Created on: Nov 13, 2019
 *      Author: alvar
 */

#ifndef ADAFRUIT_HT16K33_H_
#define ADAFRUIT_HT16K33_H_

#define HT16K33_address 0xE0
#define HT16K33_START_ADD 0x00

#include "MK64F12.h"
#include "../drivers/bits.h"
#include "../drivers/I2C.h"



void HT16k33_init(void);

void HT16k33_sendMat(volatile uint8_t *data);
void HT16k33_cleanMat(void);

#endif /* ADAFRUIT_HT16K33_H_ */
