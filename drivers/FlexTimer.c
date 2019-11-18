/**
    \file
    \brief
        This is the starter file of FlexTimer.
        In this file the FlexTimer is configured in overflow mode.
    \author J. Luis Pizano Escalante, luispizano@iteso.mx
    \date   21/03/2019
    \todo
        Add configuration structures.
 */

#include "FlexTimer.h"

static void (*FTM0_callback)(void) = 0;
static void (*FTM1_callback)(void) = 0;
static void (*FTM2_callback)(void) = 0;


uint8_t FlexTimer_get_duty_cycle(uint8_t channel)
{
    switch(channel)
    {
    case RGB_RED_CHANNEL:
        return FTM0->CONTROLS[0].CnV;
        break;
    case RGB_GREEN_CHANNEL:
        return FTM0->CONTROLS[1].CnV;
        break;
    case RGB_BLUE_CHANNEL:
        return FTM0->CONTROLS[2].CnV;
        break;
    default:
        break;
    }
    return 0;
}



void FlexTimer_update_channel_value(RGB_colors_t rgb)
{
    /**Assigns a new value for the duty cycle*/
    FTM0->CONTROLS[0].CnV = rgb.red_value;
    FTM0->CONTROLS[1].CnV = rgb.green_value;
    FTM0->CONTROLS[2].CnV = rgb.blue_value;
}


void FlexTimer_Init(void)
{


    /**Clock gating for FlexTimer*/
    SIM->SCGC6 |= SIM_SCGC6_FTM0(1);
    /**When write protection is enabled (WPDIS = 0), write protected bits cannot be written.
    * When write protection is disabled (WPDIS = 1), write protected bits can be written.*/
    FTM0->MODE |= FTM_MODE_WPDIS_MASK;
    /**Enables the writing over all registers*/
    FTM0->MODE &= ~(FTM_MODE_FTMEN_MASK);
    /**Assigning a default value for modulo register*/
    FTM0->MOD = 0xFF;
    /**Selects the Edge-Aligned PWM mode mode*/
    FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);
    FTM0->CONTROLS[1].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);
    FTM0->CONTROLS[2].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSB(1);
    /**Assign a duty cycle */
    FTM0->CONTROLS[0].CnV = 0x00;
    FTM0->CONTROLS[1].CnV = 0x00;
    FTM0->CONTROLS[2].CnV = 0x00;
    /**Configure the times*/
    FTM0->SC |= FTM_SC_CLKS(FLEX_TIMER_CLKS_1)| FTM_SC_PS(FLEX_TIMER_PS_16);
}

void FlexTimer_SingleCapture_init(void)
{
    /**Clock gating for FlexTimer*/
    SIM->SCGC6 |= SIM_SCGC6_FTM2(1);
    /**Reset Status Control register */
    FTM2->SC = 0x00;
    /*Capture on rising edge only*/
    FTM2->CONTROLS[0].CnSC |= FTM_CnSC_ELSA(1);
    /*Disable combine mode*/
    FTM2->COMBINE = 0x00;
    /*Modulo value is assigned (255)*/
    FTM2->MOD = 0xFF;
    /*Enable channel 0 interruption*/
    FTM2->CONTROLS[0].CnSC |= FTM_CnSC_CHIE_MASK;
    /*System clock is selected for FTM*/
    FTM2->SC = FTM_SC_CLKS(1)| FTM_SC_PS(0)| FTM_SC_TOIE(1) ;
}

RGB_colors_t Num_to_PWM(uint16_t adc_value)
{
    /*Offset correction from voltage divider*/
    if(MAX_VALUE_FOR_WO_OFFSET > adc_value)
    {
        adc_value += 100;
    }

    RGB_colors_t rgb;
    rgb.green_value = 0;
    rgb.blue_value = 0;
    rgb.red_value = 0;

    /*Since the blue color is less present in the color scale that represents the pH,
     * than the red or green color, we decided to divide our ADC value in 8 equal parts
     * and then give 3 to the red color, 3 to the green color and 2 for the blue color
     * and this next section is the one that ponders each of these proportions for each
     * color. Using this logic we had to define other values like MAX_RGB_VALUE,
     * RANGE1_2_DIVIDER, etc. */

    /*We implemented the next switch case because our range value (from 0 to 2^16 - 1)
     * had some inner ranges in which each RGB color behaved different depending on
     * this ranges.
     *
     * A more detailed explanation can be founded in the report.*/

    if ((LIMIT1 <= adc_value) && (LIMIT2 > adc_value))
    {

        rgb.red_value = (uint8_t) (MAX_RGB_VALUE - (adc_value / RANGE1_2_DIVIDER));
        rgb.green_value = (uint8_t) (adc_value / RANGE1_2_DIVIDER);
        rgb.blue_value = (uint8_t) NO_VALUE;
    }
    else if ((LIMIT2 <= adc_value) && (LIMIT3 > adc_value))
    {
        rgb.red_value = (uint8_t) NO_VALUE;
        rgb.green_value = (uint8_t) (MAX_RGB_VALUE - (adc_value / RANGE1_2_DIVIDER));
        rgb.blue_value = (uint8_t) (adc_value / RANGE1_2_DIVIDER);
    }
    else
    {
        rgb.red_value = (uint8_t) (adc_value / RANGE3_DIVIDER);
        rgb.green_value = (uint8_t) NO_VALUE;
        rgb.blue_value = (uint8_t) (MAX_RGB_VALUE - (adc_value / RANGE1_2_DIVIDER));
    }

    return rgb;
}

uint32_t FlexTimer_getMOD(void){
    return FTM0->MOD;
}

void FTM0_disable_counter(void)
{
    /*Module write protection is disable*/
    FTM0->MODE |= FTM_MODE_WPDIS(1);
    /*FTM counter is disable*/
    FTM0->SC |= FTM_SC_CLKS(0);
}

void FTM1_disable_counter(void)
{
    /*Module write protection is disable*/
    FTM1->MODE |= FTM_MODE_WPDIS(1);
    /*FTM counter is disable*/
    FTM1->SC |= FTM_SC_CLKS(0);
}

void FTM2_disable_counter(void)
{
    /*Module write protection is disable*/
    FTM2->MODE |= FTM_MODE_WPDIS(1);
    /*FTM counter is disable*/
    FTM2->SC |= FTM_SC_CLKS(0);
}

void FTM_callback_init(void (*handler)(void),uint8_t ftm)
{
    if (handler)
    {
        switch(ftm)
        {
        case FTM_0:
            FTM0_callback = handler;
            break;
        case FTM_1:
            FTM1_callback = handler;
            break;
        case FTM_2:
            FTM2_callback = handler;
            break;
        default:
            break;
        }
    }
}
void FTM0_IRQHandler(void)
{
    if(FTM0_callback)
    {
        FTM0_callback();
    }
}

void FTM1_IRQHandler(void)
{
    if(FTM1_callback)
    {
        FTM1_callback();
    }
}
void FTM2_IRQHandler(void)
{
    if(FTM2_callback)
    {
        FTM2_callback();
    }
}
