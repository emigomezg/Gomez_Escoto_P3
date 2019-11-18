/*
 * MCP7940M.c
 *
 *  Created on: Nov 11, 2019
 *      Author: alvar
 */


#include "MCP7940M.h"

static const uint8_t seconds_memory = 0x00;
static const uint8_t minutes_memory = 0x01;
static const uint8_t hours_memory = 0x02;

static const uint8_t day_memory = 0x04;
static const uint8_t month_memory = 0x05;
static const uint8_t year_memory = 0x06;

void MCP7940M_set_seconds(uint8_t seconds)
{
	if(seconds>=0 &&  seconds<60){
		seconds = ((seconds / 10) << 4) | (seconds % 10);
		seconds|=0x80;
		I2C_start(I2C_0);//sends start bit
		I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//writes the direction of RTC with the write bit enable
		I2C_wait(I2C_0);//waits for response in the line
		I2C_get_ack(I2C_0);//reads the acknowledge

		I2C_write_byte(I2C_0, seconds_memory);//the memory direction for seconds
		I2C_wait(I2C_0);//waits for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_write_byte(I2C_0, seconds);//set seconds and start the clock
		I2C_wait(I2C_0);//wait for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_stop(I2C_0);//sends nack and stop comunications
	}
}

void MCP7940M_set_minutes(uint8_t minutes)
{
	if(minutes>=0 &&  minutes<60){
		minutes =((minutes / 10) << 4) | (minutes % 10);
		I2C_start(I2C_0);//sends start bit
		I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//writes the direction of RTC with the write bit enable
		I2C_wait(I2C_0);//waits for response in the line
		I2C_get_ack(I2C_0);//reads the acknowledge

		I2C_write_byte(I2C_0, minutes_memory);//the memory direction for minutes
		I2C_wait(I2C_0);//waits for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_write_byte(I2C_0, minutes);//set minutes and start the clock
		I2C_wait(I2C_0);//wait for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_stop(I2C_0);//sends nack and stop comunications
	}
}

void MCP7940M_set_hours(uint8_t hours)
{
	if(hours>=0 &&  hours<24){
		hours =((hours / 10) << 4) | (hours % 10);
		I2C_start(I2C_0);//sends start bit
		I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//writes the direction of RTC with the write bit enable
		I2C_wait(I2C_0);//waits for response in the line
		I2C_get_ack(I2C_0);//reads the acknowledge

		I2C_write_byte(I2C_0, hours_memory);//the memory direction for hours
		I2C_wait(I2C_0);//waits for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_write_byte(I2C_0, hours);//set hours and start the clock
		I2C_wait(I2C_0);//wait for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_stop(I2C_0);//sends nack and stop comunications
	}
}

uint8_t MCP7940M_get_seconds(void)
{
	uint8_t data = 0x00;
	I2C_start(I2C_0);//sends start bit
	I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//sends RTC address + write bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads acknowledge

	I2C_write_byte(I2C_0, seconds_memory);//states you want to read the seconds
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads  acknowledge

	I2C_repeted_start(I2C_0);// sends re-start bit
	I2C_write_byte(I2C_0, MCP7940M_READ_ADD);//sends RTC address + read bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//read acknowledge

	I2C_tx_rx_mode(I2C_0, I2C_RECIVER);//change to receiver mode

	I2C_nack(I2C_0);//send NACK signal
	I2C_read_byte(I2C_0);//reads dummy value
	I2C_wait(I2C_0);//wait to line clear

	I2C_stop(I2C_0);//sends stop bit
	data = I2C_read_byte(I2C_0);//read real value
	data &= ~(0x80);
	return ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
}

uint8_t MCP7940M_get_minutes(void)
{
	uint8_t data = 0x00;
	I2C_start(I2C_0);//sends start bit
	I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//sends RTC address + write bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads acknowledge

	I2C_write_byte(I2C_0, minutes_memory);//states you want to read the minutes
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads  acknowledge

	I2C_repeted_start(I2C_0);// sends re-start bit
	I2C_write_byte(I2C_0, MCP7940M_READ_ADD);//sends RTC address + read bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//read acknowledge

	I2C_tx_rx_mode(I2C_0, I2C_RECIVER);//change to receiver mode

	I2C_nack(I2C_0);//send NACK signal
	I2C_read_byte(I2C_0);//reads dummy value
	I2C_wait(I2C_0);//wait to line clear

	I2C_stop(I2C_0);//sends stop bit
	data = I2C_read_byte(I2C_0);//read real value
	return  ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
}

uint8_t MCP7940M_get_hours(void)
{
	uint8_t data = 0x00;
	I2C_start(I2C_0);//sends start bit
	I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//sends RTC address + write bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads acknowledge

	I2C_write_byte(I2C_0, hours_memory);//states you want to read the hours
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads  acknowledge

	I2C_repeted_start(I2C_0);// sends re-start bit
	I2C_write_byte(I2C_0, MCP7940M_READ_ADD);//sends RTC address + read bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//read acknowledge

	I2C_tx_rx_mode(I2C_0, I2C_RECIVER);//change to receiver mode

	I2C_nack(I2C_0);//send NACK signal
	I2C_read_byte(I2C_0);//reads dummy value
	I2C_wait(I2C_0);//wait to line clear

	I2C_stop(I2C_0);//sends stop bit
	data = I2C_read_byte(I2C_0);//read real value
	(data&= ~(0xC0));
	 return  ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
}

void MCP7940M_set_date(uint8_t day, uint8_t month, uint8_t year)
{
	uint8_t day_limit = 0;
	/*Month of input date is validated*/
	if(month>=1 &&  month<13)
	{
		/*Depending on the month the limit day is validated*/
		switch (month)
		{
			case JAN:
			{
				day_limit = 31;
				break;
			}
			case FEB:
			{
				if(((year % 4 == 0) && (year % 100!= 0)) || (year % 400 == 0))
				{
					day_limit = 29;
				}
				else
					day_limit = 28;
			}
			case MAR:
			{
				day_limit = 31;
				break;
			}
			case APR:
			{
				day_limit = 30;
				break;
			}
			case MAY:
			{
				day_limit = 31;
				break;
			}
			case JUN:
			{
				day_limit = 30;
				break;
			}
			case JUL:
			{
				day_limit = 31;
				break;
			}
			case AUG:
			{
				day_limit = 31;
				break;
			}
			case SEP:
			{
				day_limit = 30;
				break;
			}
			case OCT:
			{
				day_limit = 31;
				break;
			}
			case NOV:
			{
				day_limit = 30;
				break;
			}
			case DEC:
			{
				day_limit = 31;
				break;
			}
		}
		if(day>=1 &&  day<day_limit)
			{
				day =((day / 10) << 4) | (day % 10);
				I2C_start(I2C_0);//sends start bit
				I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//writes the direction of RTC with the write bit enable
				I2C_wait(I2C_0);//waits for response in the line
				I2C_get_ack(I2C_0);//reads the acknowledge

				I2C_write_byte(I2C_0, day_memory);//the memory direction for seconds
				I2C_wait(I2C_0);//waits for acknowledge
				I2C_get_ack(I2C_0);//reads acknowledge

				I2C_write_byte(I2C_0, day);//set seconds and start the clock
				I2C_wait(I2C_0);//wait for acknowledge
				I2C_get_ack(I2C_0);//reads acknowledge

				I2C_stop(I2C_0);//sends nack and stop comunications
			}
		MCP7940M_set_month(month);
		MCP7940M_set_year(year);
	}
}

void MCP7940M_set_month(uint8_t month)
{
	if(month>=1 &&  month<13)
	{
		month =((month / 10) << 4) | (month % 10);
		I2C_start(I2C_0);//sends start bit
		I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//writes the direction of RTC with the write bit enable
		I2C_wait(I2C_0);//waits for response in the line
		I2C_get_ack(I2C_0);//reads the acknowledge

		I2C_write_byte(I2C_0, month_memory);//the memory direction for seconds
		I2C_wait(I2C_0);//waits for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_write_byte(I2C_0, month);//set seconds and start the clock
		I2C_wait(I2C_0);//wait for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_stop(I2C_0);//sends nack and stop comunications
	}
}

void MCP7940M_set_year(uint8_t year)
{
	if(year>=1 &&  year<99)
	{
		year =((year / 10) << 4) | (year % 10);
		I2C_start(I2C_0);//sends start bit
		I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//writes the direction of RTC with the write bit enable
		I2C_wait(I2C_0);//waits for response in the line
		I2C_get_ack(I2C_0);//reads the acknowledge

		I2C_write_byte(I2C_0, year_memory);//the memory direction for year
		I2C_wait(I2C_0);//waits for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_write_byte(I2C_0, year);//set seconds and start the clock
		I2C_wait(I2C_0);//wait for acknowledge
		I2C_get_ack(I2C_0);//reads acknowledge

		I2C_stop(I2C_0);//sends nack and stop comunications
	}
}

uint8_t MCP7940M_get_day(void)
{
	uint8_t data = 0x00;
	I2C_start(I2C_0);//sends start bit
	I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//sends RTC address + write bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads acknowledge

	I2C_write_byte(I2C_0, day_memory);//states you want to read the day
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads  acknowledge

	I2C_repeted_start(I2C_0);// sends re-start bit
	I2C_write_byte(I2C_0, MCP7940M_READ_ADD);//sends RTC address + read bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//read acknowledge

	I2C_tx_rx_mode(I2C_0, I2C_RECIVER);//change to receiver mode

	I2C_nack(I2C_0);//send NACK signal
	I2C_read_byte(I2C_0);//reads dummy value
	I2C_wait(I2C_0);//wait to line clear

	I2C_stop(I2C_0);//sends stop bit
	data = I2C_read_byte(I2C_0);//read real value
	return  ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
}

uint8_t MCP7940M_get_month(void)
{
	uint8_t data = 0x00;
	I2C_start(I2C_0);//sends start bit
	I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//sends RTC address + write bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads acknowledge

	I2C_write_byte(I2C_0, month_memory);//states you want to read the month
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads  acknowledge

	I2C_repeted_start(I2C_0);// sends re-start bit
	I2C_write_byte(I2C_0, MCP7940M_READ_ADD);//sends RTC address + read bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//read acknowledge

	I2C_tx_rx_mode(I2C_0, I2C_RECIVER);//change to receiver mode

	I2C_nack(I2C_0);//send NACK signal
	I2C_read_byte(I2C_0);//reads dummy value
	I2C_wait(I2C_0);//wait to line clear

	I2C_stop(I2C_0);//sends stop bit
	data = I2C_read_byte(I2C_0);//read real value
	(data&= ~(0x20));
	return  ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
}

uint8_t MCP7940M_get_year(void)
{
	uint8_t data = 0x00;
	I2C_start(I2C_0);//sends start bit
	I2C_write_byte(I2C_0, MCP7940M_WRITE_ADD);//sends RTC address + write bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads acknowledge

	I2C_write_byte(I2C_0, year_memory);//states you want to read the year
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//reads  acknowledge

	I2C_repeted_start(I2C_0);// sends re-start bit
	I2C_write_byte(I2C_0, MCP7940M_READ_ADD);//sends RTC address + read bit
	I2C_wait(I2C_0);//waits for response
	I2C_get_ack(I2C_0);//read acknowledge

	I2C_tx_rx_mode(I2C_0, I2C_RECIVER);//change to receiver mode

	I2C_nack(I2C_0);//send NACK signal
	I2C_read_byte(I2C_0);//reads dummy value
	I2C_wait(I2C_0);//wait to line clear

	I2C_stop(I2C_0);//sends stop bit
	data = I2C_read_byte(I2C_0);//read real value
	return  ((data & 0xF0) >> 4) * 10 + (data & 0x0F);
}
