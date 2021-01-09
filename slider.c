#include <avr/pgmspace.h>
#include "print.h"

#include "slider.h"

// These prescaler values are for high speed mode, ADHSM = 1
#if F_CPU == 16000000L
#define ADC_PRESCALER ((1<<ADPS2) | (1<<ADPS1))
#elif F_CPU == 8000000L
#define ADC_PRESCALER ((1<<ADPS2) | (1<<ADPS0))
#elif F_CPU == 4000000L
#define ADC_PRESCALER ((1<<ADPS2))
#elif F_CPU == 2000000L
#define ADC_PRESCALER ((1<<ADPS1) | (1<<ADPS0))
#elif F_CPU == 1000000L
#define ADC_PRESCALER ((1<<ADPS1))
#else
#define ADC_PRESCALER ((1<<ADPS0))
#endif

// some avr-libc versions do not properly define ADHSM
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#if !defined(ADHSM)
#define ADHSM (7)
#endif
#endif

void initialize_slider(void)
{
	// F7 is output, F0 is analog input
	DDRF |= 0x80;
	DDRF &= ~0x01;
	// Start with LED off
	PORTF &= ~0x80;

	// Configure ADC F0 
	unsigned char pin = 0;
	// Enable ADC
	ADCSRA = (1<<ADEN) | ADC_PRESCALER;
	// Use high speed mode
	ADCSRB = (1<<ADHSM);
	// Configure F0 as channel and use 8 bit reading
	ADMUX = (1 << REFS0) | (1<<ADLAR) | (pin & 0x1F);
	// Start the first conversion
	ADCSRA = (1<<ADEN) | ADC_PRESCALER | (1<<ADSC);
}

char read_slider(unsigned char upperLimit)
{
	unsigned char adcsra = ADCSRA;
	if (adcsra & (1<<ADSC))
	{
		print("read_slider value not ready. ADCSRA = ");
		phex(adcsra);
		print("\n");
		// -1 means not available, ignore this value
		return -1;
	}

	// Using 8 bit so only read the high register
	unsigned char readValue = ADCH;
	// Start the next conversion
	ADCSRA = (1<<ADEN) | ADC_PRESCALER | (1<<ADSC);
	// Scale to specified limit
	print("read_slider read value = ");
	phex(readValue);
	print("\n");
	unsigned int scaler = 0x100 / (upperLimit+1);
	unsigned char scaledValue = readValue / scaler;
	if (scaledValue > upperLimit)
	{
		scaledValue = upperLimit;
	}
	print("read_slider scaled value = ");
	phex(scaledValue);
	print("\n");
	return scaledValue;
}

bool get_slider_light(void)
{
	return PORTF & 0x80;
}

void set_slider_light(bool turnOn)
{
	if (turnOn)
	{
		PORTF |= 0x80;
	}
	else
	{
		PORTF &= ~0x80;
	}
}
