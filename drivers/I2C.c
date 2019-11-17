/*
 * PSI.c
 *
 *  Created on: Nov 5, 2019
 *      Author: alvar
 */

#include "I2C.h"

void I2C_clock_gating(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
		break;
		case I2C_1:
			SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
		break;
		case I2C_2:
			SIM->SCGC1 |= SIM_SCGC1_I2C2_MASK;
		break;
	}
}

void I2C_init(i2c_channel_t channel, uint32_t system_clock, uint32_t baud_rate)
{

	static const gpio_pin_control_register_t I2C_PCR = GPIO_MUX2;
	GPIO_clock_gating(GPIO_B);
	GPIO_pin_control_register(GPIO_B, bit_2, &I2C_PCR);
	GPIO_pin_control_register(GPIO_B, bit_3, &I2C_PCR);

	I2C_clock_gating(channel);
	uint32_t SCL_value = system_clock / (baud_rate * MULT);
	switch ((uint8_t) channel)
	{
		case I2C_0:
			I2C0->F |= I2C_F_ICR(SCL_value);
			I2C0->F |= I2C_F_MULT(1);
			I2C0->C1 |= I2C_C1_IICEN_MASK;
		break;
		case I2C_1:
			I2C1->F = I2C_F_ICR(SCL_value);
			I2C1->F |= I2C_F_MULT(2);
			I2C1->C1 = I2C_C1_IICEN_MASK;
		break;
		case I2C_2:
			I2C2->F = I2C_F_ICR(SCL_value);
			I2C2->F |= I2C_F_MULT(2);
			I2C2->C1 = I2C_C1_IICEN_MASK;
		break;

	}

}
uint8_t I2C_busy(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			return (I2C0->S & I2C_S_BUSY_MASK ? TRUE : FALSE);
		break;
		case I2C_1:
			return (I2C1->S & I2C_S_BUSY_MASK ? TRUE : FALSE);
		break;
		case I2C_2:
			return (I2C2->S & I2C_S_BUSY_MASK ? TRUE : FALSE);
		break;
		default:
			return FALSE;
		break;

	}
}
void I2C_mst_or_slv_mode(i2c_channel_t channel, i2c_slv_master_t mst_or_slv)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:

			if (mst_or_slv) {
				I2C0->C1 |= I2C_C1_MST_MASK;
			} else {
				I2C0->C1 &= ~I2C_C1_MST_MASK;
			}
		break;
		case I2C_1:
			I2C1->C1 |= I2C_C1_MST((uint8_t )mst_or_slv);
		break;
		case I2C_2:
			I2C2->C1 |= I2C_C1_MST((uint8_t )mst_or_slv);
		break;
	}
}
void I2C_tx_rx_mode(i2c_channel_t channel, i2c_trans_recv_t tx_or_rx)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			if (tx_or_rx) {
				I2C0->C1 |= I2C_C1_TX_MASK;
			} else {
				I2C0->C1 &= ~I2C_C1_TX_MASK;
			}
		break;
		case I2C_1:
			I2C1->C1 |= I2C_C1_TX(tx_or_rx);
		break;
		case I2C_2:
			I2C2->C1 |= I2C_C1_TX(tx_or_rx);
		break;
	}
}

void I2C_nack(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			I2C0->C1 |= I2C_C1_TXAK_MASK;
		break;
		case I2C_1:
			I2C1->C1 |= I2C_C1_TXAK_MASK;
		break;
		case I2C_2:
			I2C2->C1 |= I2C_C1_TXAK_MASK;
		break;
	}

}
void I2C_ack(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			I2C0->C1 &= ~I2C_C1_TXAK_MASK;
		break;
		case I2C_1:
			I2C1->C1 &= ~I2C_C1_TXAK_MASK;
		break;
		case I2C_2:
			I2C2->C1 &= ~I2C_C1_TXAK_MASK;
		break;
	}

}

void I2C_repeted_start(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			I2C0->C1 |= I2C_C1_RSTA_MASK;
		break;
		case I2C_1:
			I2C1->C1 |= I2C_C1_REP_START;
		break;
		case I2C_2:
			I2C2->C1 |= I2C_C1_REP_START;
		break;
		default:
		break;
	}
}
void I2C_write_byte(i2c_channel_t channel, uint8_t data)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			I2C0->D = data;
		break;
		case I2C_1:
			I2C1->D = data;
		break;
		case I2C_2:
			I2C2->D = data;
		break;
		default:
		break;
	}
}
uint8_t I2C_read_byte(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			return I2C0->D;
		break;
		case I2C_1:
			return I2C1->D;
		break;
		case I2C_2:
			return I2C2->D;
		break;
		default:
			return 0;
		break;
	}

}
void I2C_wait(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			I2C0->S |= (I2C_S_IICIF_MASK);
			while ((I2C0->S & I2C_S_IICIF_MASK) == 0)
				;

		break;
		case I2C_1:
			while ((I2C1->S & I2C_S_IICIF_MASK))
				;
		break;
		case I2C_2:
			while ((I2C2->S & I2C_S_IICIF_MASK))
				;
		break;
		default:
		break;
	}
}
uint8_t I2C_get_ack(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			return (I2C0->S & I2C_S_RXAK(TRUE) ? TRUE : FALSE);
		break;
		case I2C_1:
			return (I2C1->S & I2C_S_RXAK(TRUE) ? TRUE : FALSE);
		break;
		case I2C_2:
			return (I2C2->S & I2C_S_RXAK(TRUE) ? TRUE : FALSE);
		break;
		default:
			return FALSE;

	}
}

void I2C_start(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			//I2C0->C1 |= (I2C_C1_MST_MASK | I2C_C1_TX_MASK);
			I2C_tx_rx_mode(I2C_0, I2C_TRANSMITER);
			I2C_mst_or_slv_mode(I2C_0, I2C_MASTER);
		break;
		case I2C_1:
			I2C_tx_rx_mode(I2C_1, I2C_TRANSMITER);
			I2C_mst_or_slv_mode(I2C_1, I2C_MASTER);
		break;
		case I2C_2:
			I2C_tx_rx_mode(I2C_2, I2C_TRANSMITER);
			I2C_mst_or_slv_mode(I2C_2, I2C_MASTER);
		break;
		default:
		break;
	}
}
void I2C_stop(i2c_channel_t channel)
{
	switch ((uint8_t) channel)
	{
		case I2C_0:
			//I2C0->C1 &= ~I2C_C1_TXAK_MASK;
			I2C_tx_rx_mode(I2C_0, I2C_RECIVER);
			I2C_mst_or_slv_mode(I2C_0, I2C_SLAVE);
		break;
		case I2C_1:
			I2C_tx_rx_mode(I2C_1, I2C_RECIVER);
			I2C_mst_or_slv_mode(I2C_1, I2C_SLAVE);
		break;
		case I2C_2:
			I2C_tx_rx_mode(I2C_2, I2C_RECIVER);
			I2C_mst_or_slv_mode(I2C_2, I2C_SLAVE);
		break;
		default:
		break;
	}

}
