#include "../inc/lm3s6965.h"
#include "../inc/emp_type.h"
#include "../cpu/cpu.h"
#include "../inc/binary.h"
#include "lcd.h"
#include "../rtcs/rtcs.h"

#define DELAY_NS(x)		(x-20)*(FCPU/1000000000)/3
#define DELAY_US(x)		(x*(FCPU/1000000))/3
#define DELAY_MS(x)		(x*(FCPU/1000))/3

void lcd_write_4bit_mode(INT8U c);
BOOLEAN display_buffer_goto_xy(INT8U col_p, INT8U row_p);

INT8U display_buffer[COL_SIZE][ROW_SIZE];
INT8U col_pointer = 0;
INT8U row_pointer = 0;

void init_lcd_write_task(void)
{
	// Start task
	_start2(LCD_WRITE_TASK, MILLI_SEC(10));
}

void lcd_write_buffer()
{
	for(INT8U t_row = 0; t_row < ROW_SIZE; ++t_row)
	{
		for(INT8U t_col = 0; t_col < COL_SIZE; ++t_col)
		{
			lcd_set_cursor_position(t_row, t_col);
			set_rs_high();
			lcd_data_write(display_buffer[t_col][t_row]);
			lcd_delay_us(30);
		}
	}
}

void lcd_set_cursor_position(INT8U row, INT8U col)
{
	if((row >= 0 && row < ROW_SIZE) && (col >= 0 && col < COL_SIZE))
	{
		INT8U pos = 0;
		pos = row*0x40 + col;
		pos |= 0x80;
		set_rs_low();				
		lcd_write_4bit_mode(pos);
		lcd_delay_us(90);
	}
}

BOOLEAN lcd_function(INT8U function)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	BOOLEAN	return_value = 0;  

		set_rs_low();				// write control bytes
		lcd_write_4bit_mode(function);

	return return_value;
}

BOOLEAN display_buffer_goto_xy(INT8U col_p, INT8U row_p)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  INT8U pos;
	INT8U	return_value = 0;

	if ((col_p < COL_SIZE) && (row_p < ROW_SIZE))
	{
		pos = row_p*0x40 + col_p;
 		pos |= 0x80;
		return_value = lcd_function(pos);
  	}
	return return_value;
}

void lcd_write_task()
{
	if (col_pointer >= COL_SIZE)
	{
		// next line
		col_pointer = 0;
		row_pointer++;

		if (row_pointer >= ROW_SIZE)
		{
			// Goto start of display.
			col_pointer = 0;
			row_pointer = 0;
			display_buffer_goto_xy(col_pointer, row_pointer);
		}
		else
		{
			display_buffer_goto_xy(col_pointer,row_pointer);
		}
	}
	else
	{
		// Can't write characters to display in same tick period as control commands
		set_rs_high();			// write characters	
		lcd_write_4bit_mode(display_buffer[col_pointer][row_pointer]);
		col_pointer++;
	}
	_wait(MILLI_SEC(10));
}

void lcd_add_char_to_buffer(INT8U col, INT8U row, INT8U ch)
{
	display_buffer[col][row] = ch;
}

void lcd_add_string_to_buffer(INT8U col, INT8U row, INT8U *str)
{
	while ( *str )
	{
		display_buffer[col][row] = *str;
		col++;
		str++;
	}
}

void lcd_io(void)
{
	INT8S dummy;
	// Initialize internal clock
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	dummy = SYSCTL_RCGC2_R;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	dummy = SYSCTL_RCGC2_R;

	// Set pins as output
	GPIO_PORTA_DIR_R |= PA6;
	GPIO_PORTA_DIR_R |= PA7;

	GPIO_PORTB_DIR_R |= PB2;
	GPIO_PORTB_DIR_R |= PB3;
	GPIO_PORTB_DIR_R |= PB4;
	GPIO_PORTB_DIR_R |= PB5;
	GPIO_PORTB_DIR_R |= PB6;

	// Data enable
	GPIO_PORTA_DEN_R |= PA6;
	GPIO_PORTA_DEN_R |= PA7;

	GPIO_PORTB_DEN_R |= PB2;
	GPIO_PORTB_DEN_R |= PB3;
	GPIO_PORTB_DEN_R |= PB4;
	GPIO_PORTB_DEN_R |= PB5;
	GPIO_PORTB_DEN_R |= PB6;

	// set RS high
	set_rs_high();
	// set RW low
	CLEAR_BIT(GPIO_PORTA_DATA_R, LCD_RW);
	// Set E low
	CLEAR_BIT(GPIO_PORTB_DATA_R, LCD_E);

}

void set_rs_high()
{
	SET_BIT(GPIO_PORTA_DATA_R, LCD_RS);
}

void set_rs_low()
{
	CLEAR_BIT(GPIO_PORTA_DATA_R, LCD_RS);
}

void lcd_e_high()
{
	// tcycle = 500 ns; // pw = 230 ns;
	SET_BIT(GPIO_PORTB_DATA_R, LCD_E);
	// 230 / 20 almost 12
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
}

void lcd_e_low()
{
	// tcycle = 500 ns; // pw = 230 ns;
	CLEAR_BIT(GPIO_PORTB_DATA_R, LCD_E);
	// 230 / 20 almost 12
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
	__asm("nop");
}

void lcd_e_high_low()
{
	lcd_e_high();
	lcd_e_low();
}

void lcd_cmd_write(INT8U cmd)
{

	lcd_e_high();
	GPIO_PORTB_DATA_R &= ~(0x0F) << 2; // Clear bits (Mask)
	GPIO_PORTB_DATA_R |= (cmd & 0x0F) << 2; // Set new bits
	lcd_e_low();

	/*
	GPIO_PORTB_DATA_R = ((cmd & 0x0F)<< 2) | (GPIO_PORTB_DATA_R & ~(0x3C));
	lcd_e_high_low();
	*/
}

void lcd_data_write(INT8U data)
{
	lcd_e_high();
	GPIO_PORTB_DATA_R &= ~(0x0F) << 2; // Clear bits (Mask)
	GPIO_PORTB_DATA_R |= (data & 0xF0) >> 2; // Set high bits
	lcd_e_low();
	lcd_e_high();
	GPIO_PORTB_DATA_R &= ~(0x0F) << 2; // Clear bits (Mask)
	GPIO_PORTB_DATA_R |= (data & 0x0F) << 2; // Set low bits
	lcd_e_low();
}

void lcd_init()
{
	lcd_delay_ms(15); 		// Delay 15 ms
	set_rs_low();
	lcd_cmd_write(0x03);
	lcd_delay_ms(5);
	lcd_cmd_write(0x03);
	lcd_delay_us(100);
	lcd_cmd_write(0x03);
	lcd_delay_ms(5);

	lcd_cmd_write(0x02);	// Set 4-bit mode
	lcd_delay_us(40);
	lcd_data_write(0x28);	// 4-bit mode, 1/16 duty, 5x8 font
	lcd_delay_us(40);
	lcd_data_write(0x08); 	// display off
	lcd_delay_us(40);
	lcd_data_write(0x0C);	// display on, blink cursor on
	lcd_delay_us(40);
	lcd_data_write(0x06);	// entry mode
	lcd_delay_us(40);
	lcd_data_write(0x0F);	// home
	lcd_delay_us(40);
	lcd_data_write(0x01);	// home
	lcd_delay_us(40);

	set_rs_high();
}

void lcd_delay_us(INT32U i)
{
	i = DELAY_US(i);
	__asm(	"			mov r3, %0 	\n\t"
			"uloop: 	sub r3, #1	\n\t"
			"			cmp r3, #0	\n\t"
			"			bne uloop	\n\t"
			:
			: "r" (i));
}

void lcd_delay_ms(INT32U i)
{
	i = DELAY_MS(i);
	__asm(	"			mov r3, %0 	\n\t"
			"mloop: 	sub r3, #1	\n\t"
			"			cmp r3, #0	\n\t"
			"			bne mloop	\n\t"
			:
			: "r" (i));
}

void lcd_write_port(INT8U value)
{
	GPIO_PORTB_DATA_R = ((value & 0x0F)<< 2) | (GPIO_PORTB_DATA_R & ~(0x3C));
}

void lcd_write_4bit_mode(INT8U c)
/*****************************************************************************
*   Input    :	c: character.
*   Output   :	-
*   Function : 	write a character to the LCD in 4 bit mode
******************************************************************************/
{

	lcd_e_high();
	GPIO_PORTB_DATA_R = ((c & 0xF0)>> 2) | (GPIO_PORTB_DATA_R & ~(0x3C));
	lcd_e_low();
	lcd_e_high();
	GPIO_PORTB_DATA_R = ((c & 0x0F)<< 2) | (GPIO_PORTB_DATA_R & ~(0x3C));
	lcd_e_low();
}

void lcd_init_display(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	Initialise the LCD - put into 4 bit mode
******************************************************************************/
{
	lcd_delay_us(15000);
	set_rs_low();						// write control bytes
	lcd_write_port(0x03);  	// attention!
    lcd_e_high_low();
	lcd_delay_us(5000);
	lcd_e_high_low();
	lcd_delay_us(100);
	lcd_e_high_low();
	lcd_delay_us(5000);
	lcd_write_port(0x2);			// set 4 bit mode
	lcd_e_high_low();
	lcd_delay_us(40);
	lcd_write_4bit_mode(0x28);	// 4 bit mode, 1/16 duty, 5x8 font
	lcd_delay_us(40);
	lcd_write_4bit_mode(0x08);	// display off
	lcd_delay_us(40);
	lcd_write_4bit_mode(0x0C);	// display on, blink curson on
	lcd_delay_us(40);
	lcd_write_4bit_mode(0x06);	// entry mode
	lcd_delay_us(40);
	lcd_write_4bit_mode(0x01);	// Home
//	lcd_delay_us(30000);
	set_rs_high();			// write characters

}

void lcd_port_setup(void)
/*****************************************************************************
*   Input    : 	-
*   Output   : 	-
*   Function : 	LCD port setup.
******************************************************************************/
{
		INT8S dummy;

		// LCD_RS PA7
		// LCD_RW PA6
		// LCD_E  PB6 // Strobe
		// LCD_D4..D7 // PB2 .. PB5

		// Enable the GPIO port that is used for LCD_RW.
	  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	  // Do a dummy read to insert a few cycles after enabling the peripheral.
	  dummy = SYSCTL_RCGC2_R;
	  // Set the direction as output.
	  SET_BIT(GPIO_PORTA_DIR_R, LCD_RW);
	  // Enable the GPIO pins for digital function.
	  SET_BIT(GPIO_PORTA_DEN_R, LCD_RW);
	  // Set pin low
	  CLEAR_BIT(GPIO_PORTA_DATA_R, LCD_RW);

	  // Enable the GPIO port that is used for LCD_RS.
	  // Set the direction as output.
	  SET_BIT(GPIO_PORTA_DIR_R, LCD_RS);
	  // Enable the GPIO pins for digital function.
	  SET_BIT(GPIO_PORTA_DEN_R, LCD_RS);
	  // Set pin high
	  SET_BIT(GPIO_PORTA_DATA_R, LCD_RS);

	  // Enable the GPIO port that is used for LCD_E (Strobe).
 	  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
 	  // Do a dummy read to insert a few cycles after enabling the peripheral.
 	  dummy = SYSCTL_RCGC2_R;

	  // Set the direction as output.
	  SET_BIT(GPIO_PORTB_DIR_R, LCD_E);
	  // Enable the GPIO pins for digital function.
	  SET_BIT(GPIO_PORTB_DEN_R, LCD_E);
	  // Set pin low
	  CLEAR_BIT(GPIO_PORTB_DATA_R, LCD_E);

	  // Set the direction as output for LCD_D4 .. LCD_D7
	  GPIO_PORTB_DIR_R &=  ~(0x3C);
	  GPIO_PORTB_DIR_R |= 0x3C;
	  // Enable digital function for LCD_E and LCD_D4 .. LCD_D7
	  GPIO_PORTB_DEN_R &=  ~(0x3C);
	  GPIO_PORTB_DEN_R |= 0x3C;
}
