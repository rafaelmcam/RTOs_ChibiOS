#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include "bsp.h"

typedef uint16_t event_t;

event_t wait_for_events(void);
event_t test_for_event(event_t);

extern volatile event_t _events;
#define set_event(ev)                           \
        do {                                    \
                enter_critical_region();        \
                _events |= (1 << (ev));         \
                leave_critical_region();        \
        } while (0)

#define MAX_EVENTS 16

enum {
        EVENT0 = 0,
        EVENT1,
        EVENT2,
        EVENT3,
	EVENT4,
	EVENT5,
	EVENT6,
	EVENT7,
	EVENT8,
	EVENT9,
	EVENT10,
	EVENT11,
	EVENT12,
	EVENT13,
	EVENT14,
	EVENT15
};

#endif /* EVENTS_H */
