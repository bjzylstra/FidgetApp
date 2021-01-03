#include <avr/pgmspace.h>
#include "print.h"

#include "mode_switches.h"

void initialize_mode_switches(void)
{
	// Count direction switches are on E6,E7
	DDRE &= ~0xC0;
	// Turn on the pull-ups
	PORTE |= 0xC0;


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
	enum mode mode = count;

	return mode;
}
