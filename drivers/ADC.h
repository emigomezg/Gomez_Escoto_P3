/**
 \file ADC.h
 \brief
 This is the source file for the ADC device driver for Kinetis K64.
 It contains all the implementation for configuration functions and runtime functions.
 \author Emiliano Gomez Guerrero,
 \date  26/10/2019
 \todo
 Interrupts are not implemented in this API implementation.
 */

#ifndef ADC_H_
#define ADC_H_

#include "bits.h"
#include "MK64F12.h"

//Conversion mode macros
#define CONVERSION_8BIT  0
#define CONVERSION_12BIT 1
#define CONVERSION_10BIT 2
#define CONVERSION_16BIT 3

//Long Sample Time macros
#define EXTRA_20_CYCLES 0
#define EXTRA_12_CYCLES 1
#define EXTRA_6_CYCLES 2
#define EXTRA_2_CYCLES 3

//Sample average macros
#define AVG_4_SAMPLES 0
#define AVG_8_SAMPLES 1
#define AVG_16_SAMPLES 2
#define AVG_32_SAMPLES 3

#define LECTURE_COUNTER_COMPARISON 5250


/*! These constants are used to select an specific DAC in the different API functions*/
typedef enum {
    ADC_0,
    ADC_1
} ADC_t;

/* Since our ADC will give us a 16bit value we setup a group of 4 limits for each RGB color.
 * There are 4 limits because the 3 colors may overlap with another color and we have to know
 * with the ADC value in which range is the desired color, composed by the RGB color. Each
 * limit was obtained by dividing 2 elevated to 16, minus one between 8 and the multiply by
 * 3 to get the first range, from 0 to 24,576. For the second range we doubled 24,576 to get
 * it from 24,577 to 49,152. The final range is determined by the left values, from 49,153 to
 *  65,535. For range 1 and 2 we use a divider do adapt the value to our PWM function. For our
 *  range 3 we use another divider since range 3 is shorter.*/


#define NO_VALUE 0

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function will enable the corresponding ADC clock gate.
 \param[in]: ADC to be clock gate enable.
 \return: void
 \todo:
 */
void ADC_clk_gating(ADC_t adc);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function will initialize the corresponding ADC.
 \param[in]: ADC to be initialize.
 \return: void
 \todo:
 */
void ADC_setup(ADC_t adc);


#endif /* ADC_H_ */
