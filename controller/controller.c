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

/*****************************    Defines    *******************************/

// States
#define POT_EDIT			0 // Use the pot to edit speed
#define DIG_EDIT			1 // Use the drehimpulsgeber to edit speed

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void controller_change_state(INT8U state_changes)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	while (state_changes > 0)
	{
		switch ( rtc_state )
		{
			case POT_EDIT:
				rtc_state = DIG_EDIT;
				break;

			case DIG_EDIT:
				rtc_state = POT_EDIT;
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
	
}

/****************************** End Of Module *******************************/












