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
#include "../pot/pot.h"
#include "../cpu/cpu.h"

/*****************************    Defines    *******************************/

// States
#define POT_EDIT			0 // Use the pot to edit speed
#define DIG_EDIT			1 // Use the drehimpulsgeber to edit speed

#define CURRENT_UPDATE_DELAY  5 // How many ticks should there be between update of the current

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

void controller_write_fan_rpm(void)
/*****************************************************************************
*   Function : Writes out fans RPM to the LCD.
*****************************************************************************/
{
	INT16U new_rpm = fan_get_rpm();
	static INT16U old_rpm = 65534;

	if(old_rpm != new_rpm)
	{
		INT8U c1 = int_to_ascii((new_rpm / 10000) % 10);
		INT8U c2 = int_to_ascii((new_rpm / 1000) % 10);
		INT8U c3 = int_to_ascii((new_rpm / 100) % 10);
		INT8U c4 = int_to_ascii((new_rpm / 10) % 10);
		INT8U c5 = int_to_ascii((new_rpm / 1) % 10);
		
		if(c1 == 0x30)
		{
			c1 = 0x20;
			if(c2 == 0x30)
			{
				c2 = 0x20;
				if(c3 == 0x30)
				{
					c3 = 0x20;
					if(c4 == 0x30)
					{
						c4 = 0x20;
					}
				}
			}
		}
		
		lcd_add_char_to_buffer(0, 1, c1);
		lcd_add_char_to_buffer(1, 1, c2);
		lcd_add_char_to_buffer(2, 1, c3);
		lcd_add_char_to_buffer(3, 1, c4);
		lcd_add_char_to_buffer(4, 1, c5);
	}
	old_rpm = new_rpm;
}

void controller_write_fan_current(void)
/*****************************************************************************
*   Function : Writes out the current through the fan to the LCD.
*****************************************************************************/
{
	INT16U new_current = fan_get_current();
	static INT16U old_current = 65534;
	
	if(old_current != new_current)
	{
		
		INT8U c1 = int_to_ascii((new_current / 100) % 10);
		INT8U c2 = int_to_ascii((new_current / 10) % 10);
		INT8U c3 = int_to_ascii((new_current / 1) % 10);
		
		if(c1 == 0x30)
		{
			c1 = 0x20;
			if(c2 == 0x30)
			{
				c2 = 0x20;
			}
		}
		
		lcd_add_char_to_buffer(11, 1, c1);
		lcd_add_char_to_buffer(12, 1, c2);
		lcd_add_char_to_buffer(13, 1, c3);
	}
	old_current = new_current;
}

void controller_write_fan_ref_speed(void)
/*****************************************************************************
*   Function : Writes out the ref. speed of the fan to the LCD.
*****************************************************************************/
{
	INT8U new_ref = fan_get_ref_speed();
	static INT8U old_ref = 255;
	
	if(old_ref != new_ref)
	{
		
		INT8U c1 = int_to_ascii((new_ref / 100) % 10);
		INT8U c2 = int_to_ascii((new_ref / 10) % 10);
		INT8U c3 = int_to_ascii((new_ref / 1) % 10);
		
		if(c1 == 0x30)
		{
			c1 = 0x20;
			if(c2 == 0x30)
			{
				c2 = 0x20;
			}
		}
		
		lcd_add_char_to_buffer(5, 0, c1);
		lcd_add_char_to_buffer(6, 0, c2);
		lcd_add_char_to_buffer(7, 0, c3);
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
				lcd_add_string_to_buffer(13, 0, "DIG");
				
				// Zero the counter so we don't get old data
				zero_digiswitch_counter();
				
				controller_state = DIG_EDIT;
				break;

			case DIG_EDIT:
				lcd_add_string_to_buffer(13, 0, "POT");
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
	// Start CPU
	cpu_busy();
	INT8U state_changes = get_button_count();
	if(state_changes > 0)
	{
		controller_change_state(state_changes);
	}
	
	// If controlled by the drehimpulsgeber
	if(controller_state == DIG_EDIT)
	{
		INT8S dig_value = get_digiswitch_counter();
		
		if(dig_value < 0)
		{
			INT8U dig_val = -dig_value;
			fan_speed_down(dig_val);
		} else if(dig_value > 0)
		{
			INT8U dig_val = dig_value;
			fan_speed_up(dig_value);
		}
	}
	
	// If controlled by the pot.
	if(controller_state == POT_EDIT)
	{
		fan_set_speed(get_pot_value());
	}
	
	// Write the current
	static INT8U show_current = CURRENT_UPDATE_DELAY;
	if(show_current == 0)
	{
		show_current = CURRENT_UPDATE_DELAY;
		controller_write_fan_current();
	} else {
		show_current--;
	}
	// while (! --show_current)
	// {
	// 	show_current = CURRENT_UPDATE_DELAY;
	// }
	
	
	// Write the ref-speed
	controller_write_fan_ref_speed();

	// Write the RPM
	controller_write_fan_rpm();
	
	// Exit CPU
	cpu_idle();
	
	_wait(MILLI_SEC(100));
}


void init_controller(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	lcd_add_string_to_buffer(0, 0, "Ref: ");
	lcd_add_string_to_buffer(13, 0, "DIG");
	lcd_add_string_to_buffer(14, 1, "mA");
	lcd_add_string_to_buffer(5, 1, "RPM");
	// Start task
	_start2(CONTROLLER_TASK, MILLI_SEC(10));
}

/****************************** End Of Module *******************************/












