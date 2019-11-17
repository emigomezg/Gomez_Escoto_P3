/**
 \file
 \brief
 This is the header file for the FlexTimer divice driver.
 \author J. Luis Pizano Escalante, luispizano@iteso.mx
 \date  21/03/2019
 \todo
 Add configuration structures.
 */

#ifndef FLEXTIMER_H_
#define FLEXTIMER_H_

#include "MK64F12.h"


typedef enum{
    RGB_RED_CHANNEL,
    RGB_GREEN_CHANNEL,
    RGB_BLUE_CHANNEL
}rgb_channel_t;

typedef struct RGB_value_t{
    uint8_t red_value;
    uint8_t green_value;
    uint8_t blue_value;
}RGB_colors_t;

#define FLEX_TIMER_CLKS_0  (0U)
#define FLEX_TIMER_CLKS_1  (1U)
#define FLEX_TIMER_CLKS_2  (2U)
#define FLEX_TIMER_CLKS_3  (3U)

#define FLEX_TIMER_PS_1    (0U)
#define FLEX_TIMER_PS_2    (1U)
#define FLEX_TIMER_PS_4    (2U)
#define FLEX_TIMER_PS_8    (3U)
#define FLEX_TIMER_PS_16    (4U)
#define FLEX_TIMER_PS_32    (5U)
#define FLEX_TIMER_PS_64    (6U)
#define FLEX_TIMER_PS_128    (7U)

#define  FLEX_TIMER_DMA   0x01
#define  FLEX_TIMER_ELSA  0x04
#define  FLEX_TIMER_ELSB  0x08
#define  FLEX_TIMER_MSA   0x10
#define  FLEX_TIMER_MSB   0x20
#define  FLEX_TIMER_CHIE  0x40
#define  FLEX_TIMER_CHF   0x80

#define LIMIT1 0
#define LIMIT2 24576U
#define LIMIT3 49152U
#define LIMIT4 65535U

#define RANGE1_2_DIVIDER 96U
#define RANGE3_DIVIDER 1536U

#define MAX_RGB_VALUE 256U
#define NO_VALUE 0

#define VALUE_FOR_KNOWN_PERIOD 2624U
#define KNOWN_PERIOD .064F

#define FTM_PERIOD 0.00000009523809524F

typedef enum{
    CHANNEL_0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,
    CHANNEL_7
}channel_t;

/*Due to our voltage divider we had to add this offset maximum value to decide if we had to
 * compensate the voltage left in the voltage divider resistor, not the trimpot.*/
#define MAX_VALUE_FOR_WO_OFFSET 65435U

/*We got this value simply by dividing our maximum input value for our function (2 raised to 16)
 * by our maximum output value (3.3V) .*/
#define DIVIDER_FOR_VOLTAGE_CONVERSION 19860U

typedef enum{
    FTM_0,
    FTM_1,
    FTM_2
}ftm_module_t;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function will convert 16 bit data to a struct used by the FlexTimer PWM function.
 \param[in]: 16bit value.
 \return: RGB_value_t struct with each red, green and blue 8 bit value.
 \todo:
 */
RGB_colors_t Num_to_PWM (uint16_t adc_value);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function changes the duty cycle for PWM signals.
 \param[in]: RGB_colors_t struct.
 \return:void.
 \todo:
 */
void FlexTimer_update_channel_value(RGB_colors_t rgb);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This funcion will setup the FTM Input Capture Mode.
 \param[in]: void.
 \return: void.
 \todo:
 */
void FlexTimer_SingleCapture_init(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function will get the dytu cycle of the desired FTM0 channel.
 \param[in]: uint8_t channel value.
 \return: uint8_t duty cyle value.
 \todo:
 */
uint8_t FlexTimer_get_duty_cycle(uint8_t channel);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This  function will setup FTM0 PWM.
 \param[in]: void.
 \return: void.
 \todo:
 */
void FlexTimer_Init(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: FTM0 callback.
 \param[in]: Pointer to function, desired FTM.
 \return: void.
 \todo:
 */
void FTM_callback_init(void (*handler)(void),uint8_t ftm);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: Handler that ensures the callback has an address for FTM0 ISR.
 \param[in]: void.
 \return: void.
 \todo:
 */
void FTM0_IRQHandler(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: Handler that ensures the callback has an address for FTM1 ISR.
 \param[in]: void.
 \return: void.
 \todo:
 */
void FTM1_IRQHandler(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: Handler that ensures the callback has an address for FTM2 ISR.
 \param[in]: void.
 \return: void.
 \todo:
 */
void FTM2_IRQHandler(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function will disable FTM0 counter.
 \param[in]: void.
 \return: void.
 \todo:
 */
void FTM0_disable_counter(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function will disable FTM0 counter.
 \param[in]: void.
 \return: void.
 \todo:
 */
void FTM1_disable_counter(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function will disable FTM0 counter.
 \param[in]: void.
 \return: void.
 \todo:
 */
void FTM2_disable_counter(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 \brief: This function will return the current FTM0 MOD.
 \param[in]: void.
 \return: uint32_t.
 \todo:
 */
uint32_t FlexTimer_getMOD(void);

#endif /* FLEXTIMER_H_ */
