#include <avr/pgmspace.h>
#include "print.h"

#include "bar_leds.h"

void initialize_bar_leds(void)
{
	// Bar leds are active high so set to 0s to turn off.
	DDRD |= 0xff;
	PORTD = 0;
	DDRE |= 0x3;
	PORTE &= 0xfc;
}

void set_bar_leds(unsigned char number)
{
	print("set_bar_leds with number = ");
	phex(number);
	print("\n");
	
	// Left shift always fills with a 0 so 
	// start with all bits set and shift left by number
	// and invert
	unsigned char lowerDigit = number % 10;
	unsigned char portd_value = 0xff;
	portd_value <<= lowerDigit;
	portd_value = ~portd_value;
	PORTD = portd_value;
	
	// 9th digit is on portE bit 0
	unsigned char porte_value = 0;
	if (lowerDigit == 9)
	{
		porte_value |= 0x1;
	}
	// 10s digit is on portE bit 10
	if (number > 9)
	{
		porte_value |= 0x2;
	}
	PORTE = (PORTE & 0xfc) | porte_value;
}
