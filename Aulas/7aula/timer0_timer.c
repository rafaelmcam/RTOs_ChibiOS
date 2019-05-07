#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>


/* 
 * Constantes utilizadas no código 
 */


/* 
 * Definição de funções 
 */
void setup_timer0(void)
{
    /*
     * Para as macros abaixo e outras, veja o arquivo
     * /usr/lib/avr/include/avr/iom328p.h
     */
    OCR0A  = 250;
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = (1 << OCIE0A);
    sei();
}

/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= (1 << LED_PINNBR);
    LED_PORT |= (1 << LED_PINNBR);
    sei(); /* Seta flag principal de habilitação de interrupções */
}

/* -------------------------------------*/
/*
 * Interrupt handlers devem ser definidos com a macros
 * ISR(), que é definida no arquivo avr/interrupt.h.
 * Veja também a página
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
volatile uint16_t ms;
volatile uint16_t ms_set;
volatile uint8_t ring_flag;

ISR(TIMER0_COMPA_vect)
{
    if (ms)
        if(!--ms) {
            ms = ms_set;
            ring_flag = 1;
        }
}

/* -------------------------------------*/
int main(void)
{
    int8_t step = 25;
    const uint16_t max_delay = 500, min_delay = 100;

    ms_set = 100;
    ms = ms_set;

    setup_timer0();
    setup_main();
    
    while(1) {
        while (!ring_flag)
            ;
        cli();
        ring_flag = 0;
        ms_set += step;
        if ((ms_set > max_delay) || (ms_set < min_delay)) {
            step = -step;
            ms_set += 2*step;
        }
        sei();
        LED_PIN = 1 << LED_PINNBR;
    }

    return 0;
}
