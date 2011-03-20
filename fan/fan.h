/*
 * fan.h
 *
 *  Created on: 23/02/2011
 *      Author: NS.
 */

#ifndef FAN_H_
#define FAN_H_

INT8U fan_get_ref_speed(void);
/*****************************************************************************
*   Function : 	Returns the ref speed of the fan.
******************************************************************************/

void fan_set_speed( INT8U ds );
/*****************************************************************************
*   Function : Sets the ref-speed to ds.
*****************************************************************************/

void fan_speed_up( INT8U ds );
/*****************************************************************************
*   Function : Increase the ref-speed by ds.
*****************************************************************************/

void fan_speed_down( INT8U ds );
/*****************************************************************************
*   Function : Decrease the ref-speed by ds.
*****************************************************************************/

#endif /* FAN_H_ */
