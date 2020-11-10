#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "io.h"
#include "tmr.h"

int main(void)
{
	io_init();
	wdt_enable(WDTO_15MS);
	tmr_init();
	sei();
	set_sleep_mode(SLEEP_MODE_IDLE);
	for (;;)
	{
		wdt_reset();
		sleep_mode();
	}

	/* NOTREACHED */
	return 0;
}
