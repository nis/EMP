#include "inc/lm3s6965.h"
#include "inc/emp_type.h"
#include "cpu/cpu.h"
#include "rtcs/rtcs.h"
#include "lcd/lcd.h"
#include "digiswitch/digiswitch.h"

int main(void)
/*****************************************************************************
*   Input    : NONE
*   Output   : Returns 0
*   Function : Init hardware and then loop forever
******************************************************************************/
{
  	
	disable_global_int();
	clk_system_init();
	
	init_digiswitch();
	init_digi_button();
	
	enable_global_int();
	
	cpu_setup_measure_led();
	
	start_rtcs_scheduler();		// Call scheduler
  
	while(1);
  
  return( 0 );
}