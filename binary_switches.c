#include <avr/pgmspace.h>
#include "print.h"

#include "binary_switches.h"

void initialize_binary_switches(void)
{
	// Set B4-B7 as inputs
	DDRB &= ~0xf0;
	// Set the data on the inputs to high to enable the pull-up
	PORTB |= 0xf0;
}

unsigned char read_binary_switches(void)
{
	// Inputs are active low so invert and shift into lower 4
	unsigned char switches = (~PINB >> 4) & 0xf;

	print("read_binary_switches returns ");
	phex(switches);
	print("\n");
	return switches;
}
