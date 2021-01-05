#include <avr/pgmspace.h>
#include "print.h"

#include "mode_switches.h"

void initialize_mode_switches(void)
{
	// Count direction switches are on E6,E7
	DDRE &= ~0xC0;
	// Turn on the pull-ups
	PORTE |= 0xC0;

	// Mode switch is on F3-F6. Start them all as Inputs
	// 0 is input, 1 is output
	DDRF &= ~0x78;
	// Turn on the pull ups and start the output high
	PORTF |= 0x78;
}

enum count_direction read_direction_switch(void)
{
	// Inputs are active low so invert and shift into lower 2
	unsigned char switches = (~PINE >> 6) & 0x3;

	print("read_direction_switch returns ");
	phex(switches);
	print("\n");

	enum count_direction switchPosition = up;
	if (switches & 1)
	{
		switchPosition = down;
	}
	return switchPosition;
}

enum mode read_mode_switch(void)
{
	// Drive line must be asserted for a cycle before the read is 
	// reliable so this becomes a state machine driven by the state
	// of the DDRF.
	unsigned char current_state = DDRF & 0x30;
	unsigned char switches = 0;
	enum mode mode;

	print("read_mode_switch current_state ");
	phex(current_state);
	print("\n");

	if (current_state == 0)
	{
		// No lines driven. Drive MODE 1 low and return unknown until
		// next iteration. Next state will be 0x20 aka Drive MODE 1.
		DDRF |= 0x20;
		PORTF &= ~0x20;
		mode = unknown;
	}
	else if (current_state == 0x20)
	{
		// Driving MODE 1. Read the switches to determine count or
		// binary.
		switches = ~PINF & 0x50;
		print("read_mode_switch switches ");
		phex(switches);
		print("\n");

		PORTF |= 0x20;
		DDRF &= ~0x20;

		if (switches & 0x40)
		{
			mode = count;
		}
		else if (switches & 0x10)
		{
			mode = binary;
		}

		// If neither, Drive MODE 2 and return unknown until next iteration.
		if (switches == 0)
		{
			DDRF |= 0x10;
			PORTF &= ~0x10;
			mode = unknown;
		}
	}
	else if (current_state == 0x10)
	{
		// Driving MODE 2. Read the switches to determine binary or
		// slider.
		switches = ~PINF & 0x28;
		print("read_mode_switch switches ");
		phex(switches);
		print("\n");
		PORTF |= 0x10;
		DDRF &= ~0x10;

		if (switches & 0x08)
		{
			mode = slider;
		}
		else if (switches & 0x20)
		{
			mode = binary;
		}

		// If neither, Drive MODE 1 and return unknown until next iteration.
		if (switches == 0)
		{
			DDRF |= 0x20;
			PORTF &= ~0x20;
			mode = unknown;
		}
	}

	print("read_mode_switch returns ");
	phex(mode);

	return mode;
}
