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
#include "../cpu/cpu.h"
#include "../rtcs/rtcs.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void alive_task(void)
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	// Toggles status LED every 1 sec.
	GPIO_PORTF_DATA_R ^= 0x01;
	
	// Wait 1 sec.
	_wait(MILLI_SEC(1000));
}

void init_alive_task(void)
/*****************************************************************************
*   Function : See h-file for specification.
*****************************************************************************/
{
	// Init status led port.
  INT8S dummy;
  // Enable the GPIO port that is used for the on-board LED.
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  // Set the direction as output (PF0).  
  GPIO_PORTF_DIR_R |= 0x01;
  //SET_BIT_HIGH(GPIO_PORTF_DIR_R, PF0);
	  
	// Enable the GPIO pins for digital function (PF0 and PF1).
	GPIO_PORTF_DEN_R |= 0x01;
  //SET_BIT_HIGH(GPIO_PORTF_DEN_R, PF0);
  
	// Set pin high
  GPIO_PORTF_DATA_R |= 0x01;
	//SET_BIT_HIGH(GPIO_PORTF_DATA_R, PF0);

  // Start task
  _start2(ALIVE_TASK, MILLI_SEC(1000));
}

/****************************** End Of Module *******************************/












