#include <avr/pgmspace.h>
#include "print.h"

#include "bar_leds.h"

const unsigned char portd_for_number[];
const unsigned char porte_for_number[];

void initialize_bar_leds(void)
{
	DDRD |= 0xFF;
	DDRE |= 0x3;
}

const unsigned char portd_for_number[] PROGMEM = {
	0x0,
	0x1,
	0x3,
	0x7,
	0xf,
	0x1f,
	0x3f,
	0x7f,
	0xff,
	0xff,
	0xff
};

const unsigned char porte_for_number[] PROGMEM = {
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x1,
	0x3
};

void set_bar_leds(unsigned char number)
{
	print("set_bar_leds with number = ");
	phex(number);
	print("\n");
	
	/* Convert the number to a bit mask */
	if (number > 10)
	{
		number = 0;
	}
	
	unsigned char portd_value = pgm_read_byte(portd_for_number + number);
	PORTD = portd_value;
	
	unsigned char porte_value = pgm_read_byte(porte_for_number + number);
	PORTE = (PORTE & 0xfc) | porte_value;
}
