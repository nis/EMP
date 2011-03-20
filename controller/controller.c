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

/*****************************    Defines    *******************************/

// States
#define POT_EDIT			0 // Use the pot to edit speed
#define DIG_EDIT			1 // Use the drehimpulsgeber to edit speed

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U controller_state = DIG_EDIT;

/*****************************   Functions   *******************************/

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
}


void init_controller(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	// Start task
	_start2(CONTROLLER_TASK, MILLI_SEC(500));
}

/****************************** End Of Module *******************************/












