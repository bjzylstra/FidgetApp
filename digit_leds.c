#include <avr/pgmspace.h>
#include "print.h"

#include "digit_leds.h"

void initialize_digit_leds(void)
{
	// 7 segment is active low so set bits high to turn off LEDs
	DDRC |= 0xff;
	PORTC = 0xff;
}

// Active low so flip each bit indicated
//
//       -----  0x04
// 0x02 |     | 0x08
//       -----  0x01
// 0x40 |     | 0x10
//       -----  0x20
//
const unsigned char portc_for_number[] PROGMEM = {
	0x01, // 0
	0x67, // 1
	0x12, // 2
	0x42, // 3
	0x64, // 4
	0x48, // 5
	0x08, // 6
	0x63, // 7
	0x00, // 8
	0x40, // 9
	0x20, // A
	0x0c, // B
	0x19, // C
	0x06, // D
	0x18, // E
	0x38  // F
};

void set_digit_leds(unsigned char number)
{
	print("set_digit_leds with number = ");
	phex(number);
	print("\n");
	
	// Convert the number to a bit mask
	unsigned char portc_value = pgm_read_byte(portc_for_number + (number & 0xf));
	PORTC = portc_value;
}

bool get_decimal_point()
{
	// Active low
	return (PORTC & 0x80) == 0;
}

void set_decimal_point(bool turnOn)
{
	// Active low
	if (turnOn)
	{
		PORTC &= 0x7f;
	}
	else
	{
		PORTC |= 0x80;
	}
}
