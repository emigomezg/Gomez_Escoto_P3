/**
 \file ADC.c
 \brief
 This is the source file for the ADC device driver for Kinetis K64.
 It contains all the implementation for configuration functions and runtime functions.
 \author Emiliano Gomez Guerrero,
 \date  26/10/2019
 \todo
 Interrupts are not implemented in this API implementation.
 */


#include "ADC.h"

void ADC_clk_gating(ADC_t adc)
{
    switch (adc)
    {
    case ADC_0:
        SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
        break;
    case ADC_1:
        SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK;
        break;
    default:
        break;
    }
}

void ADC_setup (ADC_t adc)
{
    ADC_clk_gating(adc);
    switch (adc)
    {
    case ADC_0:

        ADC0->SC1[0]  &= ~ADC_SC1_ADCH_MASK;                //Channel 0 from ADC0 is selected as input

        ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK;                 //Long sample is selected to get the desired ADCK cycles
        ADC0->CFG1 |= (ADC_CFG1_MODE(CONVERSION_16BIT));   //16 bit ADC resolution mode is selected

        ADC0->CFG2 |= ADC_CFG2_ADLSTS(EXTRA_6_CYCLES);      //Long sample time select to add 6 ADCK cycles

        ADC0->SC3 |= ADC_SC3_AVGE_MASK;                     //Enable sample average
        ADC0->SC3 |= ADC_SC3_AVGS(AVG_16_SAMPLES);          //Select hardware average of 16 samples

        break;

    case ADC_1:

        ADC1->SC1[0]  &= ~ADC_SC1_ADCH_MASK;                //Channel 0 from ADC1 is selected as input

        ADC1->CFG1 |= ADC_CFG1_ADLSMP_MASK;                 //Long sample is selected to get the desired ADCK cycles
        ADC1->CFG1 |= (ADC_CFG1_MODE(CONVERSION_16BIT));  //16 bit ADC resolution mode is selected

        ADC1->CFG2 |= ADC_CFG2_ADLSTS(EXTRA_6_CYCLES);      //Long sample time select to add 6 ADCK cycles

        ADC1->SC3 |= ADC_SC3_AVGE_MASK;                     //Enable sample average
        ADC1->SC3 |= ADC_SC3_AVGS(AVG_16_SAMPLES);          //Select hardware average of 16 samples

        break;

    default:
        break;

    }
}
