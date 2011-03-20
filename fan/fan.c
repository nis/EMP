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
* 03192011  NS    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#include "../inc/lm3s6965.h"
#include "../inc/binary.h"
#include "../inc/emp_type.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U ref_speed = 0; // Speed of the fan. From 0 to 255

/*****************************   Functions   *******************************/

void fan_task(void)
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	
}

void fan_speed_up( INT8U ds )
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	if( ref_speed > (255 - ds))
	{
		ref_speed = 255;
	} else {
		ref_speed = ref_speed + ds;
	}
}

void fan_speed_down( INT8U ds )
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	if( ref_speed < ds)
	{
		ref_speed = 0;
	} else {
		ref_speed = ref_speed - ds;
	}
}
/****************************** End Of Module *******************************/












