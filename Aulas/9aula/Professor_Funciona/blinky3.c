#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "event.h"


#define MAX_CB 2
#define CB_MASK ((1 << MAX_CB) -1)

void f1(event_t e)
{
    PINB = _BV(PB5);
}

void f2(event_t e)
{
    PINB = _BV(PB4);
}

typedef void (*cb_t)(event_t);
cb_t cb_array[MAX_CB] = {f1, f2};

void setup_main(void)
{
    DDRB |= _BV(DD4) | _BV(DD5);
    sei();
}

int main(void)
{
    uint8_t ev;

    setup_timer0_timer(2000, 1);
    setup_timer2_timer(6000, 1);
    setup_main();

    while (1) {
        ev = wait_for_events();
        cb_array[ev](_BV(ev));
    }

    return 0;
}
