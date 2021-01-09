//#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "bar_leds.h"
#include "binary_leds.h"
#include "binary_switches.h"
#include "digit_leds.h"
#include "mode_switches.h"
#include "slider.h"
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

	unsigned char number = 0;
	enum mode lastModeSwitch = unknown;
	unsigned int delaySinceUpdate = 0;
	unsigned int delayBetweenUpdates = 1000;
	unsigned int delay = 100;
	while (1) {
		enum mode modeSwitch = read_mode_switch();
		if (modeSwitch == unknown)
		{
			modeSwitch = lastModeSwitch;
		}
		enum count_direction direction = read_direction_switch();
		switch (modeSwitch)
		{
			case count:
				delaySinceUpdate += delay;
				if (delaySinceUpdate >= delayBetweenUpdates)
				{
					delaySinceUpdate = 0;
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
				}
				lastModeSwitch = modeSwitch;
				set_slider_light(false);
				break;
			case binary:
				number = read_binary_switches();
				delaySinceUpdate = 0;
				lastModeSwitch = modeSwitch;
				set_slider_light(false);
				break;
			case slider:
				delaySinceUpdate = 0;
				lastModeSwitch = modeSwitch;
				char sliderValue = read_slider(9);
				if ((sliderValue & 0x80) == 0)
				{
					number = sliderValue;
				}
				set_slider_light(true);
				break;
			case unknown:
				set_slider_light(false);
				break;
		}

		set_bar_leds(number);
		set_binary_leds(number);
		set_digit_leds(number);
		set_decimal_point(number > 9);
		_delay_ms(delay);
	}
}

void initialize_io()
{
	initialize_bar_leds();
	initialize_binary_leds();
	initialize_digit_leds();
	initialize_binary_switches();
	initialize_mode_switches();
	initialize_slider();
}
