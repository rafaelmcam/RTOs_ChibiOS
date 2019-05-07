#include <avr/io.h>
#include <avr/interrupt.h>


/* -------------------------------------*/
void setup_int0(void)
{
    /*
     * Para as macros abaixo e outras, veja o arquivo
     * usr/lib/avr/include/avr/iom328p.h
     */
    EICRA = (1 << ISC00);
    EIMSK |= (1 << INT0);
    EIFR  = (1 << INTF0);
}

/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= (1 << LED_PINNBR);
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
    sei(); /* Seta flag principal de habilitação de interrupções */
}

/* -------------------------------------*/
volatile uint8_t has_changed;

ISR(INT0_vect)
{
    has_changed = 1;
}

/* -------------------------------------*/
int main(void)
{
    setup_int0();
    setup_main();

    while(1){
        if (has_changed==1){
            LED_PIN |= (1<<LED_PINNBR);
            has_changed = 0;
        }
    }
    return 0;
}
