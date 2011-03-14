/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
* 
* MODULENAME.: rtcscnf.h
*
* PROJECT....: EMP
*
* DESCRIPTION: The RTCS configuration file.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef _RTCSCNF_H
  #define _RTCSCNF_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

// Definition of RTCS timer frequency in Hz.
#define RTCS_TIMER_FREQ					50000000

// Definition of system tick in millisec.
#define T_TICK						10

// Definition of user tasks. Always start from task 0 and proceed onwards.
// If no task is required, write NULL. LAST_TASK must be set to the task
// number of the last task.
//
// Note : o Tasks are _always_ numbered from 0 and onwards.
//        o Tasks and initialisation functions are declared as external
//          automatically and assumed to be of type 'void Task(void)'.

// Task number of LAST task
#define LAST_TASK					3

#define TASK0         		alive_task
#define TASK1         		sec_tick
#define TASK2         		update_clock
#define TASK3         		lcd_write_task
#define TASK4         		NULL
#define TASK5         		NULL
#define TASK6         		NULL
#define TASK7         		NULL

#define INIT_TASK0   		init_alive_task
#define INIT_TASK1      	init_sec_tick
#define INIT_TASK2      	init_clock
#define INIT_TASK3      	init_lcd_write_task
#define INIT_TASK4      	NULL
#define INIT_TASK5      	NULL
#define INIT_TASK6      	NULL
#define INIT_TASK7      	NULL

//
// Definition of task names. For customization, change the symbol
// immediately AFTER the #define, e.g.
//
//      #define MY_TASK   0

#define ALIVE_TASK				0
#define RTC_SEC_TASK			1
#define CLOCK_TASK				2
#define LCD_WRITE_TASK			3
#define TASK4_NAME				4
#define TASK5_NAME				5
#define TASK6_NAME				6
#define TASK7_NAME				7


//
// Definition of semaphore names. For customization, change the symbol
// immediately AFTER the #define, e.g.
//
//      #define MY_SEMAPHORE    BIT7

#define SEM0										BIT0
#define SEM1										BIT1
#define SEM2										BIT2
#define SEM3										BIT3
#define SEM4        						BIT4
#define SEM5        						BIT5
#define SEM6        						BIT6
#define SEM7        						BIT7
#define SEM8        						BIT8
#define SEM9        						BIT9
#define SEM10        						BIT10
#define SEM11        						BIT11
#define SEM12        						BIT12
#define SEM13        						BIT13
#define SEM14        						BIT14
#define SEM15        						BIT15


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/
#endif
