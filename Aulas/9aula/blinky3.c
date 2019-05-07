#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "event.h"


#define MAX_CB 2
#define CB_MASK ((1 << MAX_CB) -1)

typedef void (cb_t)(event_t);
cb_t cb_array[2] = {f, f};



void f(event_t a){
    
    return (1);
    
}






void f2(event_t b){
    
    return (2);
    
}






int main(void)
{
    uint8_t i;
    event_t copy;

    setup_timer0_timer(250, 0);
    setup_timer2_timer(500, 0);

    while (1) {
        while (!events)
            ;
       // copy = events;
        for(i=0, copy=events; i<MAX_CB && copy; i++, copy>>=1)
            if (copy & 0x1) {
                cli();
                events &= CB_MASK;
                events &= ~_BV(i);
                sei();
                break;
            }
    
    
    
    cb_array[i](_BV(i));
    
    
    
    //}
    
    
    return 0;
}
 
 //cada ocorrencia de stup gerará ativacao de bits, levasse isso em conta 
