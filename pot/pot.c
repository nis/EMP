/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (EMP)
*
* MODULENAME.: pot.c
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

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U pot_value = 0; // The value of the pot.

/*****************************   Functions   *******************************/

INT8U get_pot_value(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	return pot_value;
}

void pot_task(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	if( ADC_RIS_R &&  ADC_RIS_INR3 )
	{
		INT16U data = (0x3FF & ADC_SSFIFO3_R) / 10;
		if(data > 100)
		{
			data = 100;
		}
		pot_value = data;
		ADC_PSSI_R |=ADC_PSSI_SS3;
		//pot_value = (data)*12000/1024;
	}
	
	_wait(MILLI_SEC(10));
}

void init_pot(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	// Init ADC
	
	SYSCTL_RCGC0_R |= 0x00010000; // ADC clock
	
	ADC_ACTSS_R &= ~ADC_ACTSS_ASEN3; // Disable sequenser 3
	
	ADC_SSMUX3_R |=1;
	
	ADC_SSCTL3_R |= ADC_SSCTL3_IE0 | ADC_SSCTL3_END0;
	
	ADC_ACTSS_R |= ADC_ACTSS_ASEN3; // Enable sequence 3
	
	ADC_PSSI_R |= ADC_PSSI_SS3; // Start conversion
	
	INT16U dummy;
	for(dummy=20000;dummy>1;dummy--); // Dummy, yay!
	
	// Start task
	_start2(POT_TASK, MILLI_SEC(10));
}

/****************************** End Of Module *******************************/












