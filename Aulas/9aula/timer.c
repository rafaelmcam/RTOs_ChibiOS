#include "timer.h"
#include "event.h"


uint16_t ms0, ms2, ms0_set, ms2_set;


/* -------------------------------------*/
void setup_timer0_timer(uint16_t ms, uint8_t is_one_shot)
{
    ms0 = ms;
    if(!is_ont_shot)
        ms0_set = ms;

    OCR0A  = 250;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = (1 << OCIE0A);
}


/* -------------------------------------*/
void setup_timer2_timer(uint16_t ms, uint8_t is_one_shot)
{
    ms2 = ms;
    if(is_ont_shot)
        ms2_set = 0;
    else
        ms2_set = ms;

    OCR2A  = 250;
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS21) | (1 << CS20);
    TIMSK2 = (1 << OCIE2A);
}


/* -------------------------------------*/
ISR(TIMER0_COMPA_vect)
{
    if (ms0)
        if(!--ms0) {
            ms0 = ms0_set;
            events = EVENT0;
        }
}


/* -------------------------------------*/
ISR(TIMER2_COMPA_vect)
{
    if (ms2)
        if(!--ms2) {
            ms2 = ms2_set;
            events = EVENT1;
        }
}

