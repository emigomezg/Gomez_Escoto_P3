/**
 \file
 \brief
 This is the source file for the PIT device driver for Kinetis K64.
 It contains some configuration functions and runtime functions.
 It is implemented using  CMSIS Core functions
 \author Emiliano Gomez
 \date	17/09/2019
 */

#include "PIT.h"

static pit_interrupt_flags_t g_pit_flag = {0};

static void (*pit_0_callback)(void) = 0;
static void (*pit_1_callback)(void) = 0;
static void (*pit_2_callback)(void) = 0;
static void (*pit_3_callback)(void) = 0;

void PIT_callback_init(void (*handler)(void),uint8_t pit)
{
	if (handler)
	{
		switch(pit)
		{
		case PIT_0:
			pit_0_callback = handler;
			break;
		case PIT_1:
			pit_1_callback = handler;
			break;
		case PIT_2:
			pit_2_callback = handler;
			break;
		case PIT_3:
			pit_3_callback = handler;
			break;
		default:
			break;
		}
	}
}
void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay)
{
	uint32_t LDVAL = 0;
	My_float_pit_t pit_clock = system_clock/2;
	My_float_pit_t pit_clock_period = (1/pit_clock);
	LDVAL = (uint32_t)(delay/pit_clock_period);
	LDVAL -= 1;

	PIT->CHANNEL[pit_timer].LDVAL = LDVAL;
}

void PIT_enable(void)
{
	PIT->MCR &= ~(PIT_MCR_MDIS_MASK);
	PIT->MCR |= PIT_MCR_FRZ_MASK;
}

void PIT_clock_gating(void)
{
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

void PIT_enable_interrupt(PIT_timer_t pit)
{
	PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TIE_MASK;
}

void PIT_enable_timer(PIT_timer_t pit)
{
	PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void PIT_disable_timer(PIT_timer_t pit)
{
	uint32_t disable = PIT_TCTRL_TEN_MASK;
	PIT->CHANNEL[pit].TCTRL &= ~(disable);
}


void PIT_clear_interrupt_flag(PIT_timer_t pit)
{
	switch (pit) {
	case PIT_0:
		g_pit_flag.pit_flag_0 = FALSE;
		break;

	case PIT_1:
		g_pit_flag.pit_flag_1 = FALSE;
		break;

	case PIT_2:
		g_pit_flag.pit_flag_2 = FALSE;
		break;

	case PIT_3:
		g_pit_flag.pit_flag_3 = FALSE;
		break;

	default:
		g_pit_flag.pit_flag_0 = FALSE;
		g_pit_flag.pit_flag_1 = FALSE;
		g_pit_flag.pit_flag_2 = FALSE;
		g_pit_flag.pit_flag_3 = FALSE;
		break;
	}
}

uint8_t PIT_get_interrupt_flag_status(PIT_timer_t pit)
{
	uint8_t state = 0;

	switch (pit)
	{
	case PIT_0:
		state = g_pit_flag.pit_flag_0;
		break;

	case PIT_1:
		state = g_pit_flag.pit_flag_1;
		break;

	case PIT_2:
		state = g_pit_flag.pit_flag_2;
		break;

	case PIT_3:
		state = g_pit_flag.pit_flag_3;
		break;

	default:
		break;
	}
	return state;
}

void PIT_flag_set(PIT_timer_t pit)
{
	PIT->CHANNEL[pit].TFLG |= PIT_TFLG_TIF_MASK;
	uint32_t dummyRead = PIT->CHANNEL[pit].TCTRL;
	dummyRead /= 1;
	switch (pit)
	{
	case PIT_0:
		g_pit_flag.pit_flag_0 = TRUE;
		break;
	case PIT_1:
		g_pit_flag.pit_flag_1 = TRUE;
		break;
	case PIT_2:
		g_pit_flag.pit_flag_2 = TRUE;
		break;
	case PIT_3:
		g_pit_flag.pit_flag_3 = TRUE;
		break;
	default:
		break;
	}
}

void PIT0_IRQHandler(void)
{
	PIT_flag_set(PIT_0);
	if (pit_0_callback)
	{
		pit_0_callback();
	}
}

void PIT1_IRQHandler(void)
{
	PIT_flag_set(PIT_1);
	if (pit_1_callback)
	{
		pit_1_callback();
	}
}

void PIT2_IRQHandler(void)
{
	PIT_flag_set(PIT_2);
	if (pit_2_callback)
	{
		pit_2_callback();
	}
}

void PIT3_IRQHandler(void)
{
	PIT_flag_set(PIT_3);
	if (pit_3_callback)
	{
		pit_3_callback();
	}
}
