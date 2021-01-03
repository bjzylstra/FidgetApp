//#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "bar_leds.h"
#include "binary_leds.h"
#include "binary_switches.h"
#include "digit_leds.h"
#include "mode_switches.h"
#include "usb_debug_only.h"
//#include "print.h"

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

void initialize_io(void);

int main(void)
{
	// set for 16 MHz clock
	CPU_PRESCALE(0);
	initialize_io();

	// initialize the USB, but don't wait for the host to
	// configure.
	usb_init();

	// blink morse code messages!
	unsigned char number = 0;
	while (1) {
		enum mode modeSwitch = read_mode_switch();
		enum count_direction direction = read_direction_switch();
		switch (modeSwitch)
		{
			case count:
				if (direction == up)
				{
					if (++number > 9)
					{
						number = 0;
					}
				}
				else
				{
					if (--number > 9)
					{
						number = 9;
					}
				}
				break;
			case binary:
				number = read_binary_switches();
				break;
			case slider:
				break;
		}

		set_bar_leds(number);
		set_binary_leds(number);
		set_digit_leds(number);
		set_decimal_point(number > 9);
		_delay_ms(1000);
	}
}

void initialize_io()
{
	initialize_bar_leds();
	initialize_binary_leds();
	initialize_digit_leds();
	initialize_binary_switches();
	initialize_mode_switches();
}
