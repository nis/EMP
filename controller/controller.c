/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (EMP)
*
* MODULENAME.: controller.c
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
* 03192011  NS    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#include "../inc/lm3s6965.h"
#include "../inc/binary.h"
#include "../inc/emp_type.h"
#include "../rtcs/rtcs.h"
#include "../digiswitch/digiswitch.h"
#include "../fan/fan.h"
#include "../lcd/lcd.h"

/*****************************    Defines    *******************************/

// States
#define POT_EDIT			0 // Use the pot to edit speed
#define DIG_EDIT			1 // Use the drehimpulsgeber to edit speed

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U controller_state = DIG_EDIT;

/*****************************   Functions   *******************************/

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

void controller_write_fan_ref_speed(void)
/*****************************************************************************
*   Function : Writes out the ref. speed of the fan to the LCD.
*****************************************************************************/
{
	INT8U new_ref = fan_get_ref_speed();
	static INT8U old_ref = 0;
	
	if(old_ref != new_ref)
	{
		lcd_add_char_to_buffer(5, 0, int_to_ascii((new_ref / 100) % 10));
		lcd_add_char_to_buffer(6, 0, int_to_ascii((new_ref / 10) % 10));
		lcd_add_char_to_buffer(7, 0, int_to_ascii(new_ref % 10));
	}
	old_ref = new_ref;
}

void controller_change_state(INT8U state_changes)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	while (state_changes > 0)
	{
		switch ( controller_state )
		{
			case POT_EDIT:
				lcd_add_string_to_buffer(0, 0, "DIG");
				
				// Zero the counter so we don't get old data
				zero_digiswitch_counter();
				
				controller_state = DIG_EDIT;
				break;

			case DIG_EDIT:
				lcd_add_string_to_buffer(0, 0, "POT");
				controller_state = POT_EDIT;
				break;

			default:
				break;
		}
		state_changes--;
	}
}

void controller_task(void)
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	INT8U state_changes = get_button_count();
	if(state_changes > 0)
	{
		controller_change_state(state_changes);
	}
	
	
	controller_write_fan_ref_speed();
	
	
	
	_wait(MILLI_SEC(10));
}


void init_controller(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	lcd_add_string_to_buffer(0, 0, "DIG: ");
	// Start task
	_start2(CONTROLLER_TASK, MILLI_SEC(10));
}

/****************************** End Of Module *******************************/












