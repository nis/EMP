/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (EMP)
*
* MODULENAME.: systick.c
*
* PROJECT....: EMP
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

/***************************** Include files *******************************/

#include "../inc/lm3s6965.h"
#include "../inc/binary.h"
#include "../inc/emp_type.h"
#include "../rtcs/rtcs.h"
#include "../lcd/lcd.h"

/*****************************    Defines    *******************************/

// RTC states
#define RTC_NORMAL			0
#define RTC_EDIT_HOURS		1
#define RTC_EDIT_MINUTES	2
#define RTC_EDIT_SECONDS	3

INT8U int_to_ascii(INT8U);

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U seconds 	= 0;
INT8U minutes 	= 0;
INT8U hours		= 11;

BOOLEAN show_delim = 0;

INT8U  rtc_state =  RTC_NORMAL;
/*****************************   Functions   *******************************/

void change_state(INT8U state_changes)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	while (state_changes > 0)
	{
		switch ( rtc_state )
		{
			case RTC_NORMAL:
				rtc_state = RTC_EDIT_HOURS;
				break;

			case RTC_EDIT_HOURS:
				rtc_state = RTC_EDIT_MINUTES;
				break;

			case RTC_EDIT_MINUTES:
				rtc_state = RTC_EDIT_SECONDS;
				break;

			case RTC_EDIT_SECONDS:
				rtc_state = RTC_NORMAL;
				break;

			default:
				break;
		}
		state_changes--;
	}
}

void init_clock(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	// Setup the first line of the LCD for clock use:
	lcd_add_string_to_buffer(3, 0, "Time 00:00:00");
	// Start task
	  _start2(CLOCK_TASK, MILLI_SEC(500));
}

void update_normal(void)
{
	lcd_add_char_to_buffer(15, 0, get_ascii_char_at(0)); // LS seconds
	lcd_add_char_to_buffer(14, 0, get_ascii_char_at(1)); // MS seconds
	lcd_add_char_to_buffer(12, 0, get_ascii_char_at(2)); // LS mintutes
	lcd_add_char_to_buffer(11, 0, get_ascii_char_at(3)); // MS mintutes
	lcd_add_char_to_buffer(9, 0, get_ascii_char_at(4)); // LS hours
	lcd_add_char_to_buffer(8, 0, get_ascii_char_at(5)); // MS hours
	
	if (show_delim)
	{
		lcd_add_char_to_buffer(13, 0, 0x3A);
		lcd_add_char_to_buffer(10, 0, 0x3A);
	 	show_delim = 0;
	} else {
	 	lcd_add_char_to_buffer(13, 0, 0x20);
	 	lcd_add_char_to_buffer(10, 0, 0x20);
	 	show_delim = 1;
	 }
}

void update_edit_hours(void)
{
	lcd_add_char_to_buffer(13, 0, 0x3A);
	lcd_add_char_to_buffer(10, 0, 0x3A);
	if (show_delim)
	{
		lcd_add_char_to_buffer(9, 0, get_ascii_char_at(4)); // LS hours
		lcd_add_char_to_buffer(8, 0, get_ascii_char_at(5)); // MS hours
	 	show_delim = 0;
	} else {
	 	lcd_add_char_to_buffer(9, 0, 255); // LS hours
		lcd_add_char_to_buffer(8, 0, 255); // MS hours
	 	show_delim = 1;
	}
}

void update_edit_minutes(void)
{
	lcd_add_char_to_buffer(13, 0, 0x3A);
	lcd_add_char_to_buffer(10, 0, 0x3A);
	if (show_delim)
	{
		lcd_add_char_to_buffer(12, 0, get_ascii_char_at(2)); // LS mintutes
		lcd_add_char_to_buffer(11, 0, get_ascii_char_at(3)); // MS mintutes
	 	show_delim = 0;
	} else {
	 	lcd_add_char_to_buffer(12, 0, 255); // LS hours
		lcd_add_char_to_buffer(11, 0, 255); // MS hours
	 	show_delim = 1;
	}
}

void update_edit_seconds(void)
{
	lcd_add_char_to_buffer(13, 0, 0x3A);
	lcd_add_char_to_buffer(10, 0, 0x3A);
	if (show_delim)
	{
		lcd_add_char_to_buffer(15, 0, get_ascii_char_at(0)); // LS seconds
		lcd_add_char_to_buffer(14, 0, get_ascii_char_at(1)); // MS seconds
	 	show_delim = 0;
	} else {
	 	lcd_add_char_to_buffer(15, 0, 255); // LS hours
		lcd_add_char_to_buffer(14, 0, 255); // MS hours
	 	show_delim = 1;
	}
}

void update_clock(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	INT8U state_changes = get_button_count();
	if(state_changes > 0)
	{
		update_normal(); // To get rid of any lingering boxes in the LCD.
		change_state(state_changes);
	}
	
	INT8S edit_counter = get_digiswitch_counter();
	
	switch ( rtc_state )
	{
		case RTC_NORMAL:
			update_normal();
			break;
			
		case RTC_EDIT_HOURS:
			adjust_hours(edit_counter);
			update_edit_hours();
			break;
			
		case RTC_EDIT_MINUTES:
			adjust_minutes(edit_counter);
			update_edit_minutes();
			break;
			
		case RTC_EDIT_SECONDS:
			adjust_seconds(edit_counter);
			update_edit_seconds();
			break;
			
		default:
			break;
	}

	
	// Wait 1/2 sec.
	_wait(MILLI_SEC(330));
}

void adjust_hours(INT8S a) {
	while ( a != 0 )
	{
		if(a > 0)
		{
			// Increase the hours
			if(hours >= 23)
			{
				hours = 0;
			} else {
				hours++;
			}
			a--;
		} else {
			// Decrease the hours
			if(hours <= 0)
			{
				hours = 23;
			} else {
				hours--;
			}
			a++;
		}
	}
}

void adjust_minutes(INT8S a) {
	while ( a != 0 )
	{
		if(a > 0)
		{
			// Increase the minutes
			if(minutes >= 59)
			{
				minutes = 0;
			} else {
				minutes++;
			}
			a--;
		} else {
			// Decrease the minutes
			if(minutes <= 0)
			{
				minutes = 59;
			} else {
				minutes--;
			}
			a++;
		}
	}
}

void adjust_seconds(INT8S a) {
	while ( a != 0 )
	{
		if(a > 0)
		{
			// Increase the seconds
			if(seconds >= 59)
			{
				seconds = 0;
			} else {
				seconds++;
			}
			a--;
		} else {
			// Decrease the seconds
			if(seconds <= 0)
			{
				seconds = 59;
			} else {
				seconds--;
			}
			a++;
		}
	}
}

void init_sec_tick(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	// Start task
	  _start2(RTC_SEC_TASK, MILLI_SEC(1000));
}

void sec_tick(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	if(rtc_state == RTC_NORMAL)
	{
		if(seconds >= 59)
		{
			seconds = 0;

			if(minutes >= 59)
			{
				minutes = 0;

				if(hours >= 23)
				{
					hours = 0;
				} else {
					hours++;
				}

			} else {
				minutes++;
			}

		} else {
			seconds++;
		}
	}
	
	// Wait 1 sec.
	_wait(MILLI_SEC(1000));
}

INT8U get_ascii_char_at(INT8U pos)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	INT8U return_value = 0x00;
	
	switch ( pos )
	{
		case 0:
			return_value = int_to_ascii(seconds % 10);
			break;
		case 1:
			return_value = int_to_ascii(seconds / 10);
			break;
		case 2:
			return_value = int_to_ascii(minutes % 10);
			break;
		case 3:
			return_value = int_to_ascii(minutes / 10);
			break;
		case 4:
			return_value = int_to_ascii(hours % 10);
			break;
		case 5:
			return_value = int_to_ascii(hours / 10);
			break;
		default:
			return_value = int_to_ascii(0);
	}
	
	return return_value;
}

INT8U int_to_ascii(INT8U number)
/*****************************************************************************
*   Function : Accepts an int < 10 and returns the ascii value for that number
*****************************************************************************/
{
	if(number < 10)
	{
		return 0x30 + number;
	} else {
		return 0x30;
	}
}
/****************************** End Of Module *******************************/












