#include <avr/pgmspace.h>
#include "print.h"

#include "binary_leds.h"

void initialize_binary_leds(void)
{
	// Binary leds are active high so set to 0s to turn off.
	DDRB |= 0xf;
	PORTB &= ~0xf;
}

void set_binary_leds(unsigned char number)
{
	print("set_binary_leds with number = ");
	phex(number);
	print("\n");
	
	// Direct bit map of the number.
	unsigned char portb_value = PORTB & ~0xf;
	portb_value |= (number & 0xf);
	PORTB = portb_value;
}
