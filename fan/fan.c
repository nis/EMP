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
#include "../pwm/pwm.h"
#include "../rtcs/rtcs.h"

/*****************************    Defines    *******************************/

#define MAX_SPEED  100
#define MIN_SPEED  0

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U ref_speed = MIN_SPEED; 	// Speed of the fan. From 0 to 100
INT16U fan_current = 0;			// Current in the fan in mA

/*****************************   Functions   *******************************/

INT16U fan_get_current()
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	return fan_current;
}

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
	// Set speed if changed
	static old_speed;
	if(old_speed != ref_speed)
	{
		pwm_set_duty_cycle(ref_speed);
	}
	old_speed = ref_speed;
	
	// Read the current
	if (ADC_RIS_R && ADC_RIS_INR2) {
		INT16U data = (0x3FF & ADC_SSFIFO2_R);
		ADC_ISC_R |= ADC_ISC_IN2;
		
		fan_current = data;
		
		ADC_PSSI_R |= ADC_PSSI_SS2; // Enable the ADC for next time
	}
	
	_wait(MILLI_SEC(10));
}

void init_fan(void)
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	// Init the ADC to check the current through the fan.

	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC; // ADC enabled.

	INT8U dummy = SYSCTL_RCGC0_R; // Dummy! Yay!

	ADC_ACTSS_R &= ~ADC_ACTSS_ASEN2; // Disable sequencer two.

	ADC_SSMUX2_R |= 4;

	ADC_SSCTL2_R |=  ADC_SSCTL2_IE0 |ADC_SSCTL2_END0;
	
	ADC_ACTSS_R |= ADC_ACTSS_ASEN2;

	ADC_PSSI_R |=ADC_PSSI_SS2|ADC_PSSI_SS3;
	
	// Start task
	_start2(FAN_TASK, MILLI_SEC(10));
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












