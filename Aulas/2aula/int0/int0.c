#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>


/* -------------------------------------*/
void setup_int0(void)
{
    /*
     * Para as macros abaixo e outras, veja o arquivo
     * usr/lib/avr/include/avr/iom328p.h
     */
    EICRA = (1<< ISC00);
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
/*
 * Interrupt handlers devem ser definidos com a macros
 * ISR(), que é definida no arquivo avr/interrupt.h.
 * Veja também a página
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
ISR(INT0_vect)
{
    LED_PIN |= (1 << LED_PINNBR);
}

/* -------------------------------------*/
int main(void)
{
    setup_int0();
    setup_main();

    PORTB |= (1<<PB5);

    while(1)
        ;

    return 0;
}
