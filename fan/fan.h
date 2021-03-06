/*
 * fan.h
 *
 *  Created on: 23/02/2011
 *      Author: NS.
 */

#ifndef FAN_H_
#define FAN_H_

void fan_task(void);
/*****************************************************************************
*   Function : Changes the PWM cycle according to the ref-speed.
*****************************************************************************/

void init_fan(void);
/*****************************************************************************
*   Function : Initializes the fan. Doesn't do much.
*****************************************************************************/

INT16U fan_get_rpm(void);
/*****************************************************************************
*   Function : Returns the fans RPM.
*****************************************************************************/

INT16U fan_get_current(void);
/*****************************************************************************
*   Function : Returns the current through the fan.
*****************************************************************************/

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
