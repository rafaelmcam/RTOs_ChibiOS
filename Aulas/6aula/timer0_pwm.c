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
    /*
     * Primeiro setamos o pino OC0B como saída
     */
    DDRD |= (1 << PD5);  /* OC0B para a placa Nano */
    
    /*
     * Agora configuramos Timer0
     */
    OCR0A  = 160;
    OCR0B  = 48;
    TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << WGM02) | (1 << CS00);
}

/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= (1 << LED_PINNBR);
    sei(); /* Seta flag principal de habilitação de interrupções */
}

/* -------------------------------------*/
void delay_ms(uint16_t ms)
{
    uint16_t i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<4000; j++)
            _NOP();
}

/* -------------------------------------*/
int main(void)
{
    setup_timer0();
    setup_main();

    while(1)
        ;

    return 0;
}
