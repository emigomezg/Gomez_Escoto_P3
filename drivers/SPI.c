/**
	\file
	\brief
		This file contains the implementation of the SPI drivers.

	\author Alvaro Escoto Ramirez
	\date	19/09/2019
	\todo
 */


#include "SPI.h"
#include "MK64F12.h"


uint32_t SPI_clear_MDIS(spi_channel_t channel);
void SPI_restore_MDIS(spi_channel_t channel, uint32_t);



void SPI_init(const spi_config_t* SPI_Config)
{
	SPI_clk(SPI_Config->spi_channel);
	SPI_set_master(SPI_Config->spi_channel,SPI_Config->spi_master);

	GPIO_clock_gating(SPI_Config->spi_gpio_port.gpio_port_name);

	GPIO_pin_control_register(SPI_Config->spi_gpio_port.gpio_port_name,
			SPI_Config->spi_gpio_port.spi_clk, &(SPI_Config->pin_config));
	GPIO_pin_control_register(SPI_Config->spi_gpio_port.gpio_port_name,SPI_Config->spi_gpio_port.spi_sout, &(SPI_Config->pin_config));

	SPI_frame_size(SPI_Config->spi_channel, SPI_Config->spi_frame_size);
	SPI_baud_rate(SPI_Config->spi_channel, SPI_Config->spi_baudrate);
	SPI_msb_first(SPI_Config ->spi_channel, SPI_Config->spi_lsb_or_msb);
	SPI_clock_polarity(SPI_Config->spi_channel, SPI_Config->spi_polarity);
	SPI_clock_phase(SPI_Config->spi_channel, SPI_Config->spi_phase);
	SPI_fifo(SPI_Config->spi_channel, SPI_Config->spi_enable_fifo);

	SPI_enable(SPI_Config->spi_channel);
}


void SPI_stop_tranference(spi_channel_t channel)
{
	switch(channel)
	{
	case SPI_0:
		SPI0->SR |= SPI_SR_EOQF_MASK;
		SPI0->MCR |= SPI_MCR_HALT_MASK;
		break;
	case SPI_1:
		SPI1->SR |= SPI_SR_EOQF_MASK;
		SPI1->MCR |= SPI_MCR_HALT_MASK;
		break;
	case SPI_2:
		SPI2->SR |= SPI_SR_EOQF_MASK;
		SPI2->MCR |= SPI_MCR_HALT_MASK;
		break;
	}
}

void SPI_start_tranference(spi_channel_t channel)
{
	switch(channel)
	{
	case SPI_0:
		SPI0->MCR = (SPI0->MCR& (~(SPI_MCR_HALT_MASK)));
		SPI0->SR &= ~(1<<28);
		break;
	case SPI_1:
		SPI1->SR &= ~(1<<28);
		SPI1->MCR = (SPI1->MCR& (~(SPI_MCR_HALT_MASK)));
		break;
	case SPI_2:
		SPI2->SR &= ~(1<<28);
		SPI2->MCR = (SPI2->MCR& (~(SPI_MCR_HALT_MASK)));
		break;
	}
}

uint8_t SPI_tranference(spi_channel_t channel, uint8_t data)
{
	uint8_t received_data = 0;
	SPI0->PUSHR = (data)| SPI_PUSHR_EOQ_MASK;
	while ((SPI0->SR & SPI_SR_TCF_MASK) == 0);
	SPI0->SR |= SPI_SR_TCF_MASK;
	received_data = SPI0->POPR & 0xff;
	return (received_data);
}


void SPI_msb_first(spi_channel_t channel, spi_lsb_or_msb_t msb)
{
	if (SPI_MSB == msb)
	{
		switch(channel)
		{
		case SPI_0:
			SPI0->CTAR[0] |=  SPI_CTAR_LSBFE(msb);
			break;
		case SPI_1:
			SPI1->CTAR[0] |= SPI_CTAR_LSBFE(msb);
			break;
		case SPI_2:
			SPI2->CTAR[0] |= SPI_CTAR_LSBFE(msb);
			break;
		}
	}
	else
	{
		switch(channel)
		{
		case SPI_0:
			SPI0->CTAR[0] &= ~SPI_CTAR_LSBFE_MASK;
			break;
		case SPI_1:
			SPI1->CTAR[0] &= ~SPI_CTAR_LSBFE_MASK;
			break;
		case SPI_2:
			SPI2->CTAR[0] &= ~SPI_CTAR_LSBFE_MASK;
			break;
		}
	}
}

void SPI_baud_rate(spi_channel_t channel, uint32_t baud_rate)
{
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[0] |= baud_rate;
		break;
	case SPI_1:
		SPI1->CTAR[0] |= baud_rate;
		break;
	case SPI_2:
		SPI2->CTAR[0] |= baud_rate;
		break;
	}
}

void SPI_clock_phase(spi_channel_t channel, spi_phase_t cpha)
{
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[0] |= SPI_CTAR_CPHA(cpha);
		break;
	case SPI_1:
		SPI1->CTAR[0] |= SPI_CTAR_CPHA(cpha);
		break;
	case SPI_2:
		SPI2->CTAR[0] |= SPI_CTAR_CPHA(cpha);
		break;
	}
}

void SPI_clock_polarity(spi_channel_t channel, spi_polarity_t cpol )
{
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[0] |= SPI_CTAR_CPOL(cpol);
		break;
	case SPI_1:
		SPI1->CTAR[0] |= SPI_CTAR_CPOL(cpol);
		break;
	case SPI_2:
		SPI2->CTAR[0] |= SPI_CTAR_CPOL(cpol);
		break;
	}
}



void SPI_clk(spi_channel_t channel){
	switch(channel)
	{
	case SPI_0:
		SIM->SCGC6 |= SPI0_CLOCK_GATING;
		break;
	case SPI_1:
		SIM->SCGC6 |= SPI1_CLOCK_GATING;
		break;
	case SPI_2:
		SIM->SCGC3 |= SPI2_CLOCK_GATING;
	}
}
void SPI_fifo(spi_channel_t channel, spi_enable_fifo_t enable_or_disable)
{
	SPI_clear_MDIS(channel);

	switch(channel)
	{
	case SPI_0:
		if(SPI_ENABLE_FIFO == enable_or_disable)
			{
				SPI0->MCR &= ~ SPI_MCR_DIS_RXF_MASK; /** Enables RX FIFO */
				SPI0->MCR &= ~ SPI_MCR_DIS_TXF_MASK; /** Enables TX FIFO */
			}
			else
			{
				SPI0->MCR |=  SPI_MCR_DIS_RXF_MASK; /** Disables RX FIFO */
				SPI0->MCR |=  SPI_MCR_DIS_TXF_MASK; /** Disables TX FIFO */
			}
	break;
	case SPI_1:
		if(SPI_ENABLE_FIFO == enable_or_disable)
			{
				SPI1->MCR &= ~ SPI_MCR_DIS_RXF_MASK; /** Enables RX FIFO */
				SPI1->MCR &= ~ SPI_MCR_DIS_TXF_MASK; /** Enables TX FIFO */
			}
			else
			{
				SPI1->MCR |=  SPI_MCR_DIS_RXF_MASK; /** Disables RX FIFO */
				SPI1->MCR |=  SPI_MCR_DIS_TXF_MASK; /** Disables TX FIFO */
			}
	break;
	case SPI_2:
		if(SPI_ENABLE_FIFO == enable_or_disable)
			{
				SPI2->MCR &= ~ SPI_MCR_DIS_RXF_MASK; /** Enables RX FIFO */
				SPI2->MCR &= ~ SPI_MCR_DIS_TXF_MASK; /** Enables TX FIFO */
			}
			else
			{
				SPI2->MCR |=  SPI_MCR_DIS_RXF_MASK; /** Disables RX FIFO */
				SPI2->MCR |=  SPI_MCR_DIS_TXF_MASK; /** Disables TX FIFO */
			}
	break;
	}
}
uint32_t SPI_clear_MDIS(spi_channel_t channel)
{
	uint32_t temp = 0;

	switch(channel)
	{
	case SPI_0:
		 temp = SPI0->MCR & SPI_MCR_MDIS_MASK;
		 SPI0->MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	case SPI_1:
		 temp = SPI1->MCR & SPI_MCR_MDIS_MASK;
		 SPI1->MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	case SPI_2:
		 temp = SPI2->MCR & SPI_MCR_MDIS_MASK;
		 SPI2->MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	}
	return temp;
}

void SPI_restore_MDIS(spi_channel_t channel, uint32_t mdis)
{
	switch(channel)
	{
	case SPI_0:
		 SPI0->MCR |= mdis;
		break;
	case SPI_1:
		 SPI1->MCR |= mdis;
		break;
	case SPI_2:
		 SPI2->MCR |= mdis;
		break;
	}
}
void SPI_set_master(spi_channel_t channel,spi_master_t is_master)
{
	switch(channel)
	{
	case SPI_0:
		SPI0->MCR |= SPI_MCR_MSTR_MASK ;
		break;
	case SPI_1:
		SPI1->MCR |= SPI_MCR_MSTR_MASK ;
		break;
	case SPI_2:
		SPI2->MCR |= SPI_MCR_MSTR_MASK ;
		break;
	}
}


void SPI_sendOneByte(uint8_t Dato)
{
	SPI0->PUSHR = (Dato);
	while(0 == (SPI0->SR && SPI_SR_TCF_MASK));
	SPI0->SR |= SPI_SR_TCF_MASK;
}
void SPI_enable(spi_channel_t channel)
{
	switch(channel)
	{
	case SPI_0:
		SPI0->MCR &= ~SPI_MCR_MDIS_MASK;
		break;
	case SPI_1:
		SPI1->MCR &= ~SPI_MCR_MDIS_MASK;
		break;
	case SPI_2:
		SPI2->MCR &= ~SPI_MCR_MDIS_MASK;
		break;
	}
}

void SPI_frame_size(spi_channel_t channel, uint32_t frame_size)
{
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[0] &= ~SPI_CTAR_FMSZ_MASK;
		SPI0->CTAR[0] |= frame_size;
		break;
	case SPI_1:
		SPI1->CTAR[0] &= ~SPI_CTAR_FMSZ_MASK;
		SPI1->CTAR[0] |= frame_size;
		break;
	case SPI_2:
		SPI2->CTAR[0] &= ~SPI_CTAR_FMSZ_MASK;
		SPI2->CTAR[0] |= frame_size;
		break;
	}
}
