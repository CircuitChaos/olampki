#pragma once

#include <avr/io.h>

static inline void io_init(void)
{
	PORTB	= 0xf7;
	DDRB	= 0x08;
}

static inline void io_set_led(void)
{
	PORTB	|= _BV(3);
}

static inline void io_clr_led(void)
{
	PORTB	&= (uint8_t) ~_BV(3);
}

static inline uint8_t io_get_btn(void)
{
	return !(PINB & _BV(4));
}
