/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: events.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090215  MoH   Module created.
*
*****************************************************************************/

#ifndef EVENTS_H_
#define EVENTS_H_

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/
// Events.
// -------

// General Events
// --------------
#define GE_NO_EVENT      0

// Button Events
// -------------
#define DIGISWITCH_CW   1
#define DIGISWITCH_CCW  2
#define DIGISWITCH_PUSH 3

// Timer Events
// ------------
#define TE_TIMEOUT      10
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/
#endif /*EVENTS_H_*/
