#ifndef mode_switches_h__
#define mode_switches_h__

#include <stdbool.h>

enum count_direction
{
	up,
	down
};

enum mode
{
	count,
	binary,
	slider
};

void initialize_mode_switches(void);
enum count_direction read_direction_switch(void);
enum mode read_mode_switch(void);

#endif
