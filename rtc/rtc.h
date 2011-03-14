/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (ECP)
*
* MODULENAME.: rtc.h
*
* PROJECT....: ECP
*
* DESCRIPTION:
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _RTC_H
  #define _RTC_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

void change_state(INT8U);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Changes the state of the RTC state_changes times.
*****************************************************************************/

void init_clock(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Initializes the clock_task in RTCS.
*****************************************************************************/

void update_clock(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Updates the clock on the LCD. Call twice a second.
				Blinks the ":"
*****************************************************************************/

void init_sec_tick(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Initializes the sec_tick in RTCS.
*****************************************************************************/

void sec_tick(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Call once a second. Increments the time.	
*****************************************************************************/

INT8U get_ascii_char_at(INT8U pos);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Returns the ascii character for the number at pos position.
				0 for LS second
				1 for MS second
				2 for LS minute
				3 for MS minute
				4 for LS hour
				5 for MS hour
*****************************************************************************/

/****************************** End Of Module *******************************/
#endif
