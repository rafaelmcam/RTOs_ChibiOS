#include "event.h"
#include "bsp.h"


volatile event_t _events;

event_t test_for_event(event_t ev)
{
	return _events & (1 << ev);
}


event_t wait_for_events(void)
{
	uint8_t ev;
	event_t copy;

	while(!_events)
		;

	for(ev=0, copy=_events; ev<MAX_EVENTS && copy; ev++, copy>>=1)
		if (copy & 0x1) {
			enter_critical_region();
			_events &= ~(1 << ev);
			leave_critical_region();
			break;
		}

	return ev;
}
