#include <avr/io.h>
#include <avr/interrupt.h>
#include "event.h"


volatile event_t events;


event_t wait_for_events(void)
{
    uint8_t ev;
    event_t copy;

    while(!events)
        ;

    for(ev=0, copy=events; ev<MAX_EVENTS && copy; ev++, copy>>=1)
        if (copy & 0x1) {
            cli();
            events &= ~_BV(ev);
            sei();
            break;
        }
    
    return ev;
}
