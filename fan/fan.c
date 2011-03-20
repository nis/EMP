/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (EMP)
*
* MODULENAME.: fan.c
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

#define MAX_SPEED  100
#define MIN_SPEED  0

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U ref_speed = MIN_SPEED; // Speed of the fan. From 0 to 100

/*****************************   Functions   *******************************/

INT8U fan_get_ref_speed()
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	return ref_speed;
}

void fan_task(void)
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	
}

void fan_set_speed( INT8U ds )
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	if(ds <= MAX_SPEED && ds >= MIN_SPEED)
	{
		ref_speed = ds;
	}
}

void fan_speed_up( INT8U ds )
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	if( ref_speed > (MAX_SPEED - ds))
	{
		ref_speed = MAX_SPEED;
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
		ref_speed = MIN_SPEED;
	} else {
		ref_speed = ref_speed - ds;
	}
}
/****************************** End Of Module *******************************/












