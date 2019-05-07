#include <avr/io.h>
#include <avr/interrupt.h>
#include "event.h"


volatile event_t _events;

void set_event(event_t ev)
{
	_events |= ev;
}

event_t test_for_event(event_t ev)
{
	return _events & ev;
}


event_t wait_for_events(void)
{
	uint8_t ev;
	event_t copy;

	while(!_events)
		;

	for(ev=0, copy=_events; ev<MAX_EVENTS && copy; ev++, copy>>=1)
		if (copy & 0x1) {
			cli();
			_events &= ~_BV(ev);
			sei();
			break;
		}

	return _BV(ev);
}
