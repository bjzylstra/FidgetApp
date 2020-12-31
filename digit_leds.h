#ifndef digit_leds_h__
#define digit_leds_h__

#include <stdbool.h>

void initialize_digit_leds(void);
void set_digit_leds(unsigned char number);
bool get_decimal_point(void);
void set_decimal_point(bool turnOn);

#endif
