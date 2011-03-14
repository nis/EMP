/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: schedule.c
*
* PROJECT....: ECP
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
#include "emp_type.h"
#include "glob_def.h"
#include "systick.h"
#include "rtcs.h"
#include "rtcscnf.h"

#include "../inc/lm3s6965.h"

/*****************************    Defines    *******************************/
enum 	task_states 			{RUNNING, STOPPED};

#define _READY(a)    		((task_state[a] == RUNNING) && (task_time[a] == 0))

#define ENTER_CRITICAL()	__asm("cpsid i");
#define EXIT_CRITICAL()		__asm("cpsie i");


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

// functionpointer to init task function

// const rom void (*init_task[LAST_TASK+1])(void) = 
void (*init_task[LAST_TASK+1])(void) = \
{   \

#if (LAST_TASK >= 0)
 INIT_TASK0  
#endif

#if (LAST_TASK >= 1)
, INIT_TASK1  
#endif

#if (LAST_TASK >= 2)
, INIT_TASK2 
#endif

#if (LAST_TASK >= 3)
, INIT_TASK3 
#endif

#if (LAST_TASK >= 4)
, INIT_TASK4 
#endif

#if (LAST_TASK >= 5)
, INIT_TASK5 
#endif

#if (LAST_TASK >= 6)
, INIT_TASK6 
#endif

#if (LAST_TASK >= 7)
, INIT_TASK7 
#endif
};

 

// functionpointer to task function
void (*task[LAST_TASK+1])(void) = \
{   \

#if (LAST_TASK >= 0)
 TASK0
#endif
#if (LAST_TASK >= 1)
, TASK1  
#endif
#if (LAST_TASK >= 2)
, TASK2 
#endif
#if (LAST_TASK >= 3)
, TASK3 
#endif
#if (LAST_TASK >= 4)
, TASK4 
#endif
#if (LAST_TASK >= 5)
, TASK5 
#endif
#if (LAST_TASK >= 6)
, TASK6 
#endif
#if (LAST_TASK >= 7)
, TASK7 
#endif
};


// Task: wait counter in ticks
INT16U task_time[LAST_TASK+1];
// Task: state.
INT8U task_state[LAST_TASK+1];
// contains current status of semaphores.
INT16U semaphores = 0;

INT8U	current_task = 0;
INT8U	rtcs_i;


/*****************************   Functions   *******************************/


void _wait(INT16U time)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	DISABLE_SYSTICK_INT;
	if (task_state[current_task] == RUNNING); 
	{
		task_time[current_task] = time;
	}
	ENABLE_SYSTICK_INT;
}

void _signal_sem(INT16U sem)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	ENTER_CRITICAL();
	semaphores |= sem;
	EXIT_CRITICAL();
}

void _clear_sem(INT16U sem)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	ENTER_CRITICAL();
	semaphores &= (~sem);
	EXIT_CRITICAL();
}


INT8U _test_sem(INT16U sem)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	INT8U return_value = 0;
	
	ENTER_CRITICAL();
	return_value = ((semaphores & sem) != 0);
	EXIT_CRITICAL();

	return return_value;
}

INT16U _time_status(INT8U task)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	INT16U return_value; 
	
	DISABLE_SYSTICK_INT;	
	return_value = task_time[task];
	ENABLE_SYSTICK_INT;

	return return_value;
}

void _stop(INT8U task_number)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	DISABLE_SYSTICK_INT;
	task_state[task_number] = STOPPED;
	ENABLE_SYSTICK_INT;
}


void _start2(INT8U task_number, INT16U time)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	DISABLE_SYSTICK_INT;	
	if (task_state[task_number] == STOPPED) 
	{ 
		task_state[task_number] = RUNNING; 
		task_time[task_number] = time;
	}
	ENABLE_SYSTICK_INT;
}

void task_setup(void)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Init all tasks to STOPPED state 	
******************************************************************************/
{
	INT8U	i = 0;
	
	for (i = 0; i < LAST_TASK+1; i++)
	{
		task_time[i] = 0;
		task_state[i] = STOPPED;
	}
}


void init_tasks(void)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Call the init task function - set in rtoscnf.h 	
******************************************************************************/
{

	INT8U	i = 0;

	
	for (i = 0; i < LAST_TASK+1; i++)
	{
		if ((*init_task[i]) != NULL )
		{
			current_task = i;
			// Call init_task function
			(*init_task[i])();
		}
	}	
}


INT16S start_rtcs_scheduler(void)
/*****************************************************************************
*   Input    :	-
*   Output   :	-
*   Function :	The RTCS scheduler 
******************************************************************************/
{
	// contains temp status of semaphores.
	// INT16U semaphores_task = 0;
	
	ENTER_CRITICAL();
	task_setup();
	init_tasks();
	systick_init();
	EXIT_CRITICAL();
	
	while(1)
	{
		if (systick_get())
		{
			systick_decrement();
			for (rtcs_i = 0; rtcs_i < LAST_TASK+1; rtcs_i++)
			{
				if ((task_state[rtcs_i] == RUNNING) && (task_time[rtcs_i] > 0))
 			  	{
					task_time[rtcs_i]--;
				}	
			}	
			
			for (rtcs_i = 0; rtcs_i < LAST_TASK+1; rtcs_i++)
			{
				if _READY(rtcs_i)
				{
					task_time[rtcs_i] = 0;
					current_task = rtcs_i;
					// Call task.
					(*task[rtcs_i])();
				}
			}
		} // if (tick_flag)
	} // while(!)
	return 0;
}

/****************************** End Of Module *******************************/












