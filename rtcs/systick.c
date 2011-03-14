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
#include "rtcscnf.h"
#include "emp_type.h"
#include "glob_def.h"
#include "systick.h"

#include "../inc/lm3s6965.h"

/*****************************    Defines    *******************************/
#define ENTER_CRITICAL()	__asm("cpsid i");
#define EXIT_CRITICAL()		__asm("cpsie i");

#define SYSTICK_RELOAD_VALUE (((RTCS_TIMER_FREQ/1000)*T_TICK)-1)

#if (SYSTICK_RELOAD_VALUE > 0xFFFFFF)
	#error "RTCS timer reload value to high"
#endif

// Missing definemens in lm3s6965.h file
#define NVIC_INT_CTRL_PEND_SYST  	0x04000000  // Pend a systick int
#define NVIC_INT_CTRL_UNPEND_SYST 0x02000000  // Unpend a systick int

#define SYSTICK_PRIORITY		0x7E

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U	tick = 0;

/*****************************   Functions   *******************************/

void systick_isr(void)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	// Hardware clears systick int reguest
	tick++;
}


void systick_init(void)
{
	// Systick control and status register, NVIC_ST_CTRL_R
	// Systick reload register, NVIC_ST_RELOAD_R
	// Systick current value register, NVIC_ST_CURRENT_R
	// SYSTICK Calibration Value Register, NVIC_ST_CAL_R

	// Disable systick timer
	NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE);
	// Set current systick counter to reload value
	NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;
	// Set Reload value, Systick reload register 
	NVIC_ST_RELOAD_R = SYSTICK_RELOAD_VALUE;
	// NVIC systick setup, vector number 15
	// Clear pending systick interrupt request
	NVIC_INT_CTRL_R |= NVIC_INT_CTRL_UNPEND_SYST;
	// Set systick priority to 0x10, first clear then set. 
	NVIC_SYS_PRI3_R &= ~(NVIC_SYS_PRI3_TICK_M);
	NVIC_SYS_PRI3_R |= (NVIC_SYS_PRI3_TICK_M & (SYSTICK_PRIORITY<<NVIC_SYS_PRI3_TICK_S));
	// Select systick clock source, Use core clock
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;
	// Enable systick interrupt
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;
	// Enable and start timer
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}


INT8U systick_get(void)
{
	INT8U	return_value = 0;
	
	ENTER_CRITICAL();
	return_value = tick;
	EXIT_CRITICAL();

	return return_value;
}

void systick_decrement(void)
{
	ENTER_CRITICAL();
	tick--;
	EXIT_CRITICAL();
}

/****************************** End Of Module *******************************/












