/**
 \file
 \brief
 This is the source file for the GPIO device driver for Kinetis K64.
 It contains all the implementation for configuration functions and runtime functions.
 i.e., this is the application programming interface (API) for the GPIO peripheral.
 \author J. Luis Pizano Escalante, luispizano@iteso.mx
 \date   18/02/2019
 \todo
 Interrupts are not implemented in this API implementation.
 */

#include "GPIO.h"



#define GPIO_CLOCKA  GPIO_CLOCK_GATING_PORTA
#define GPIO_CLOCKB  GPIO_CLOCK_GATING_PORTB
#define GPIO_CLOCKC  GPIO_CLOCK_GATING_PORTC
#define GPIO_CLOCKD  GPIO_CLOCK_GATING_PORTD
#define GPIO_CLOCKE  GPIO_CLOCK_GATING_PORTE


static void (*port_a_callback)(void) = 0;
static void (*port_b_callback)(void) = 0;
static void (*port_c_callback)(void) = 0;
static void (*port_d_callback)(void) = 0;
static void (*port_e_callback)(void) = 0;



void GPIO_clear_interrupt(gpio_port_name_t port_name)
{
	switch (port_name)/** Selecting the GPIO for clock enabling*/
	{
	case GPIO_A: /** GPIO A is selected*/
		PORTA->ISFR = 0xFFFFFFFF;
		break;
	case GPIO_B: /** GPIO B is selected*/
		PORTB->ISFR = 0xFFFFFFFF;
		break;
	case GPIO_C: /** GPIO C is selected*/
		PORTC->ISFR = 0xFFFFFFFF;
		break;
	case GPIO_D: /** GPIO D is selected*/
		PORTD->ISFR = 0xFFFFFFFF;
		break;
	default: /** GPIO E is selected*/
		PORTE->ISFR = 0xFFFFFFFF;
		break;
	} // end switch
}

uint8_t GPIO_clock_gating(gpio_port_name_t port_name)
{
	switch (port_name)/** Selecting the GPIO for clock enabling*/
	{
	case GPIO_A: /** GPIO A is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
		break;
	case GPIO_B: /** GPIO B is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
		break;
	case GPIO_C: /** GPIO C is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
		break;
	case GPIO_D: /** GPIO D is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
		break;
	case GPIO_E: /** GPIO E is selected*/
		SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
		break;
	default: /**If doesn't exist the option*/
		return (FALSE);
	} // end switch
	/**Successful configuration*/
	return (TRUE);
} // end function

uint8_t GPIO_pin_control_register(gpio_port_name_t port_name, uint8_t pin,
		const gpio_pin_control_register_t* pin_control_register)
{
	switch (port_name)
	{
	case GPIO_A:/** GPIO A is selected*/
		PORTA->PCR[pin] = *pin_control_register;
		break;
	case GPIO_B:/** GPIO B is selected*/
		PORTB->PCR[pin] = *pin_control_register;
		break;
	case GPIO_C:/** GPIO C is selected*/
		PORTC->PCR[pin] = *pin_control_register;
		break;
	case GPIO_D:/** GPIO D is selected*/
		PORTD->PCR[pin] = *pin_control_register;
		break;
	case GPIO_E: /** GPIO E is selected*/
		PORTE->PCR[pin] = *pin_control_register;
	default:/**If doesn't exist the option*/
		return (FALSE);
		break;
	}
	/**Successful configuration*/
	return (TRUE);
}

void GPIO_write_port(gpio_port_name_t port_name, uint32_t data) {
	switch (port_name)
	{
	case GPIO_A:
		GPIOA->PDOR = data;
		break;
	case GPIO_B:
		GPIOB->PDOR = data;
		break;
	case GPIO_C:
		GPIOC->PDOR = data;
		break;
	case GPIO_D:
		GPIOD->PDOR = data;
		break;
	case GPIO_E:
		GPIOE->PDOR = data;
		break;
	default:
		break;
	}
}
uint32_t GPIO_read_port(gpio_port_name_t port_name)
{
	switch (port_name)
	{
	case GPIO_A:
		return GPIOA->PDIR;
	case GPIO_B:
		return GPIOB->PDIR;
	case GPIO_C:
		return GPIOC->PDIR;
	case GPIO_D:
		return GPIOD->PDIR;
	case GPIO_E:
		return GPIOE->PDIR;
	default:
		return -1;
	}
}
uint8_t GPIO_read_pin(gpio_port_name_t port_name, uint8_t pin)
{
	uint8_t temp = 0x00;

	switch (port_name)
	{
	case GPIO_A:
		temp = ((GPIOA->PDIR) & (1 << pin)) >> pin;
		break;
	case GPIO_B:
		temp = ((GPIOB->PDIR) & (1 << pin)) >> pin;
		break;
	case GPIO_C:
		temp = ((GPIOC->PDIR) & (1 << pin)) >> pin;
		break;
	case GPIO_D:
		temp = ((GPIOD->PDIR) & (1 << pin)) >> pin;
		break;
	case GPIO_E:
		temp = ((GPIOE->PDIR) & (1 << pin)) >> pin;
		break;
	default:
		temp = 0x00;
	}
	if (TRUE == temp)
		return TRUE;
	return FALSE;
}
void GPIO_set_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch (port_name)
	{
	case GPIO_A:
		GPIOA->PSOR = 1 << pin;
		break;
	case GPIO_B:
		GPIOB->PSOR = 1 << pin;
		break;
	case GPIO_C:
		GPIOC->PSOR = 1 << pin;
		break;
	case GPIO_D:
		GPIOD->PSOR = 1 << pin;
		break;
	case GPIO_E:
		GPIOE->PSOR = 1 << pin;
		break;
	default:
		break;
	}
}
void GPIO_clear_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch (port_name)
	{
	case GPIO_A:
		GPIOA->PCOR = 1 << pin;
		break;
	case GPIO_B:
		GPIOB->PCOR = 1 << pin;
		break;
	case GPIO_C:
		GPIOC->PCOR = 1 << pin;
		break;
	case GPIO_D:
		GPIOD->PCOR = 1 << pin;
		break;
	case GPIO_E:
		GPIOE->PCOR = 1 << pin;
		break;
	default:
		break;
	}
}
void GPIO_toogle_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch (port_name)
	{
	case GPIO_A:
		GPIOA->PTOR = 1 << pin;
		break;
	case GPIO_B:
		GPIOB->PTOR = 1 << pin;
		break;
	case GPIO_C:
		GPIOC->PTOR = 1 << pin;
		break;
	case GPIO_D:
		GPIOD->PTOR = 1 << pin;
		break;
	case GPIO_E:
		GPIOE->PTOR = 1 << pin;
		break;
	default:
		break;
	}
}
void GPIO_data_direction_port(gpio_port_name_t port_name, uint32_t direction)
{
	switch (port_name)
	{
	case GPIO_A:
		GPIOA->PDDR = direction;
		break;
	case GPIO_B:
		GPIOB->PDDR = direction;
		break;
	case GPIO_C:
		GPIOC->PDDR = direction;
		break;
	case GPIO_D:
		GPIOD->PDDR = direction;
		break;
	case GPIO_E:
		GPIOE->PDDR = direction;
	default:
		break;
	}
}
void GPIO_data_direction_pin(gpio_port_name_t port_name, uint8_t state,
		uint8_t pin)
{
	switch (port_name)
	{
	case GPIO_A:
		GPIOA->PDDR = ((GPIOA->PDDR & (~(1 << pin))) | (state << pin));
		break;
	case GPIO_B:
		GPIOB->PDDR = ((GPIOB->PDDR & (~(1 << pin))) | (state << pin));
		break;
	case GPIO_C:
		GPIOC->PDDR = ((GPIOC->PDDR & (~(1 << pin))) | (state << pin));
		break;
	case GPIO_D:
		GPIOD->PDDR = ((GPIOD->PDDR & (~(1 << pin))) | (state << pin));
		break;
	case GPIO_E:
		GPIOE->PDDR = ((GPIOE->PDDR & (~(1 << pin))) | (state << pin));
		break;
	}
}
void GPIO_sw2_begin(uint8_t state)
{
	gpio_pin_control_register_t sw_pcr_config1 = (GPIO_MUX1 | GPIO_PE | GPIO_PS);
	gpio_pin_control_register_t sw_pcr_config2 = (GPIO_MUX1 | GPIO_PS | GPIO_PE| INTR_FALLING_EDGE);

	GPIO_clock_gating(GPIO_C);
	if (state == PULLIN)
	{
		GPIO_pin_control_register(GPIO_C, bit_6, &sw_pcr_config1);
	}
	else
	{
		GPIO_pin_control_register(GPIO_C, bit_6, &sw_pcr_config2);
	}

	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, bit_6);
}

void GPIO_sw3_begin(uint8_t state)
{
	gpio_pin_control_register_t sw_pcr_config1 = (GPIO_MUX1 | GPIO_PE | GPIO_PS);
	gpio_pin_control_register_t sw_pcr_config2 = (GPIO_MUX1 | GPIO_PS | GPIO_PE| INTR_FALLING_EDGE);

	GPIO_clock_gating(GPIO_A);

	if (state == 0)
	{
		GPIO_pin_control_register(GPIO_A, bit_4, &sw_pcr_config1);
	}
	else
	{
		GPIO_pin_control_register(GPIO_A, bit_4, &sw_pcr_config2);
	}

	GPIO_data_direction_pin(GPIO_A, GPIO_INPUT, bit_4);
}

uint8_t GPIO_sw_state(uint8_t sw_number)
{
	switch (sw_number)
	{
	case GPIO_SW2:
		return !GPIO_read_pin(GPIO_C, bit_6);
		break;
	case GPIO_SW3:
		return !GPIO_read_pin(GPIO_A, bit_4);
	default:
		return 0;
	}
}

uint8_t GPIO_sw_until_release(uint8_t sw_number) {
	if (sw_number == GPIO_SW2)
	{
		do
		{
			if (TRUE == GPIO_read_pin(GPIO_C, bit_6))
				break;
		}
		while (TRUE);
	}
	else
	{
		do
		{
			if (TRUE == GPIO_read_pin(GPIO_A, bit_4))
				break;
		}
		while (TRUE);
	}
	return TRUE;
}

void GPIO_callback_init(void (*handler)(void),uint8_t port)
{
	if (handler)
	{
		switch(port)
		{
		case GPIO_A:
			port_a_callback = handler;
			break;
		case GPIO_B:
			port_b_callback = handler;
			break;
		case GPIO_C:
			port_c_callback = handler;
			break;
		case GPIO_D:
			port_d_callback = handler;
			break;
		case GPIO_E:
			port_e_callback = handler;
		default:
			break;
		}
	}
}

void PORTA_IRQHandler(void)
{
	GPIO_clear_interrupt(GPIO_A);
	if (port_a_callback)
	{
			port_a_callback();
	}
}

void PORTB_IRQHandler(void)
{
	GPIO_clear_interrupt(GPIO_B);
	if (port_b_callback)
	{
			port_b_callback();
	}
}

void PORTC_IRQHandler(void)
{
	GPIO_clear_interrupt(GPIO_C);
	if (port_c_callback)
	{
			port_c_callback();
	}
}

void PORTD_IRQHandler(void)
{
	GPIO_clear_interrupt(GPIO_D);
	if (port_d_callback)
	{
			port_d_callback();
	}
}
void PORTE_IRQHandler(void)
{
	GPIO_clear_interrupt(GPIO_E);
	if (port_e_callback)
	{
			port_e_callback();
	}
}

