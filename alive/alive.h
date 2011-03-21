/*
 * fan.h
 *
 *  Created on: 23/02/2011
 *      Author: NS.
 */

#ifndef ALIVE_H_
#define ALIVE_H_

void init_alive_task(void);
/*****************************************************************************
*   Function : Changes the PWM cycle according to the ref-speed.
*****************************************************************************/

void alive_task(void);
/*****************************************************************************
*   Function : Initializes the fan. Doesn't do much.
*****************************************************************************/

#endif /* ALIVE_H_ */
