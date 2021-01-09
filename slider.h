#ifndef slider_h__
#define slider_h__

#include <stdbool.h>

void initialize_slider(void);
char read_slider(unsigned char upperLimit);
bool get_slider_light(void);
void set_slider_light(bool turnOn);

#endif
