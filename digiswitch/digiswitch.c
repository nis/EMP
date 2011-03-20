/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (ECP)
*
* MODULENAME.: digiswitch.c
*
* PROJECT....: Opgave8
*
* DESCRIPTION:
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 091120  MTT,MD,KRP    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#include "../inc/lm3s6965.h"
#include "../inc/emp_type.h"
#include "../inc/binary.h"
#include "../lcd/lcd.h"

/*****************************    Defines    *******************************/
#define PB1		1
#define PC4		4
#define CLEAR_DIGI_SWITCH_INT_REQUEST {GPIO_PORTB_ICR_R |= 0b00000010;}
#define ENABLE_DIGI_SWITCH_INT {GPIO_PORTB_IM_R |= 0b00000010;}
#define DISABLE_DIGI_SWITCH_INT	{GPIO_PORTB_IM_R &= ~(0b00000010);}
#define CLEAR_DIGI_BUTTON_INT_REQUEST {GPIO_PORTC_ICR_R |= 0b00100000;}
#define ENABLE_DIGI_BUTTON_INT {GPIO_PORTC_IM_R |= 0b00100000;}
#define DISABLE_DIGI_BUTTON_INT {GPIO_PORTC_IM_R &= ~(0b00100000);}
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8S digi_switch_counter = 0;
INT8U button_counter=0;
INT16U dummy;

/*****************************   Functions   *******************************/

void zero_digiswitch_counter(void)
/*****************************************************************************
*   Function : 	Returns the current counter and zeroes it.
******************************************************************************/
{
	digi_switch_counter = 0;
}

INT8S get_digiswitch_counter(void)
/*****************************************************************************
*   Function : 	Returns the current counter and zeroes it.
******************************************************************************/
{
	INT8S return_value = digi_switch_counter;
	zero_digiswitch_counter();
	return return_value;
}

INT8U get_button_count(void)
/*****************************************************************************
*   Function : 	Returns the number of times the button was pressed.
******************************************************************************/
{
	INT8U return_value = button_counter;
	button_counter = 0;
	return return_value;
}

void enable_digi_button() 
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	enable digibutton_interrupt
******************************************************************************/
{
	CLEAR_DIGI_BUTTON_INT_REQUEST;
	ENABLE_DIGI_BUTTON_INT;
}

void digi_button_port_setup(void) 
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	Digi button port setup
******************************************************************************/
{
	// Enable the GPIO port D that is used for the comparator1.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;

	// Do a dummy read to insert a few cycles after enabling the peripheral.
	dummy = SYSCTL_RCGC2_R;

	// Set the direction as input PD7  
GPIO_PORTC_DIR_R&= ~(0b00100000);

// Enable the GPIO pins for digital function PB2-4 and PD2-5 
GPIO_PORTC_DEN_R |=0b00100000;

// Disable open drain
GPIO_PORTC_ODR_R &= ~(0b00100000);

// Disable alternative function
GPIO_PORTC_AFSEL_R &= ~(0b00100000);
}

void digi_button_int_setup(void) 
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	digibutton terrupt setup
******************************************************************************/
{

	GPIO_PORTC_IS_R &= ~(0b00100000);

	GPIO_PORTC_IBE_R &= ~(0b00100000); // Let IEV decide
	
	GPIO_PORTC_IEV_R |=0b00100000; // Single edge, the last one.

	CLEAR_DIGI_BUTTON_INT_REQUEST;

	// NVIC setup
	// program NVIC, vector number 18, Interrupt Number = 2
	// Clear pending interrupt
	NVIC_UNPEND0_R |= NVIC_UNPEND0_INT2;
	// Set Priority to 0x10, first clear then set. 
	NVIC_PRI0_R &= ~(NVIC_PRI0_INT3_M);
	NVIC_PRI0_R |= (NVIC_PRI0_INT3_M & (0x10<<NVIC_PRI0_INT3_S));
	// enable NVIC interrupt
	NVIC_EN0_R |= NVIC_EN0_INT2;

	// Enable int
	ENABLE_DIGI_BUTTON_INT;
}

void portc_isr(void) 
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	port c interrupt routine
******************************************************************************/
{
	
	button_counter++;
	//GPIO_PORTG_DATA_R ^=1;
	
	CLEAR_DIGI_BUTTON_INT_REQUEST
}

void init_digi_button(void) 
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	initialiser digibutton
******************************************************************************/
{
	digi_button_port_setup();
	digi_button_int_setup();
	//disable_digi_button();
}

void portb_isr(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	port b interrupt routine
******************************************************************************/
{
	if (TEST_BIT_HIGH(GPIO_PORTB_MIS_R, PB1))
	{
		if (TEST_BIT_HIGH(GPIO_PORTB_DATA_R,PB1) == (TEST_BIT_HIGH(GPIO_PORTC_DATA_R,PC4)>>3))
		{
			// CW
			digi_switch_counter++;
			
		}
		else
		{
			// CCW
			digi_switch_counter--;
		}
	}
		
	// Clear interrupt request
	CLEAR_DIGI_SWITCH_INT_REQUEST;
}

void digi_switch_int_setup(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	setup digiswitch interrupts
******************************************************************************/
{
	// Interrupt on Port PB1

	// Edge trigger
	GPIO_PORTB_IS_R &= ~(0b00000010);
	// both edges
	GPIO_PORTB_IBE_R |= 0b00000010;

	// negative 	edge
	// GPIO_PORTB_IEV_R &= ~(0b00000010);

	// Clear interrupt request
	CLEAR_DIGI_SWITCH_INT_REQUEST;

	// NVIC setup
	// program NVIC, vector number 17, Interrupt Number = 1
	// Clear pending interrupt
	NVIC_UNPEND0_R |= NVIC_UNPEND0_INT1;
	// Set Priority to 0x10, first clear then set. 
	NVIC_PRI0_R &= ~(NVIC_PRI0_INT1_M);
	NVIC_PRI0_R |= (NVIC_PRI0_INT1_M & (0x10<<NVIC_PRI0_INT1_S));
	// enable NVIC interrupt
	NVIC_EN0_R |= NVIC_EN0_INT1;

	// Enable int
	ENABLE_DIGI_SWITCH_INT;
}

void digi_switch_port_setup(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	set up ports for digiswitch
******************************************************************************/
{
	INT8U dummy;
	// Enable the GPIO port D and B that is used for the keyboard.
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOB;

	// Do a dummy read to insert a few cycles after enabling the peripheral.
	dummy = SYSCTL_RCGC2_R;

	// Set the direction as input PB1 PC4-5  
	GPIO_PORTB_DIR_R&= ~(0b00000010);
	GPIO_PORTC_DIR_R &= ~(0b00110000);

	// Enable the GPIO pins for digital function PB2-4 and PD2-5 
	GPIO_PORTB_DEN_R |= 0b00000010;
	GPIO_PORTC_DEN_R |= 0b00110000;

	// Disable open drain
	GPIO_PORTC_ODR_R &= ~(0b00110000);
	GPIO_PORTB_ODR_R &= ~(0b00000010);

	// Disable alternative function
	GPIO_PORTC_AFSEL_R &= ~(0b00110000);
	GPIO_PORTB_AFSEL_R &= ~(0b00000010);

	// Disable internal pull-up   
	GPIO_PORTC_PUR_R &= ~(0b00110000);
	GPIO_PORTB_PUR_R &= ~(0b00000010);
}

void init_digiswitch()
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	initialize digiswitch
******************************************************************************/
{
	digi_switch_port_setup();
	digi_switch_int_setup();
	//disable_digi_switch();
}