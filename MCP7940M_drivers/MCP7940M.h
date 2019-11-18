/*
 * MCP7940M.h
 *
 *  Created on: Nov 11, 2019
 *      Author: alvar
 */

#ifndef MCP7940M_H_
#define MCP7940M_H_
#include "../drivers/I2C.h"
#include "MK64F12.h"

typedef enum month_number{
	JAN = 1,
	FEB = 2,
	MAR = 3,
	APR = 4,
	MAY = 5,
	JUN = 6,
	JUL = 7,
	AUG = 8,
	SEP = 9,
	OCT = 10,
	NOV = 11,
	DEC = 12
}month_number_t;

#define MCP7940M_WRITE_ADD 0xDE
#define MCP7940M_READ_ADD  0xDF
/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it configures the seconds on RTC and start the time
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 	 It's also recommended to use this as last step
  	 \param[in] second in uint8_t format
  	 \return void
  */
uint8_t MCP7940M_set_seconds(uint8_t seconds);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it configures the minutes on RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] minutes in uint8_t format
  	 \return void
  */
uint8_t MCP7940M_set_minutes(uint8_t minutes);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it configures the hours on RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] hours in uint8_t format
  	 \return void
  */
uint8_t MCP7940M_set_hours(uint8_t hours);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it gets the minutes from RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] void
  	 \return uint8_t
  */
uint8_t MCP7940M_get_seconds(void);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it return the minutes from the RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] void
  	 \return uint8_t
  */
uint8_t MCP7940M_get_minutes(void);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it returns the hours from the RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] void
  	 \return uint8_t
  */
uint8_t MCP7940M_get_hours(void);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it configures the date (day, month and year) on RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] day in uint8_t format, month in uint8_t format
  	 	 	 	 year in uint8_t format
  	 \return void
  */
uint8_t MCP7940M_set_date(uint8_t day, uint8_t month, uint8_t year);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it configures the month on RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] day in uint8_t format
  	 \return void
  */
uint8_t MCP7940M_set_month(uint8_t month);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it configures the year on RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] day in uint8_t format
  	 \return void
  */
uint8_t MCP7940M_set_year(uint8_t year);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 it sets the date to the RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] void
  	 \return uint8_t
  */
uint8_t MCP7940M_get_day(void);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	  it sets the month to the RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] void
  	 \return uint8_t
  */
uint8_t MCP7940M_get_month(void);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	  it sets the year to the RTC
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] void
  	 \return uint8_t
  */
uint8_t MCP7940M_get_year(void);

#endif /* MCP7940M_H_ */
