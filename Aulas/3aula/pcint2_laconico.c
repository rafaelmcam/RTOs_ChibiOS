#include <avr/io.h>
#include <avr/interrupt.h>


/* -------------------------------------*/
uint8_t state;

void setup_pcint2(void)
{
    /*
     * Para as macros abaixo e outras, veja o arquivo
     * usr/lib/avr/include/avr/iom328p.h
     */
    PCICR = (1 << PCIE2);
    PCIFR = (1 << PCIF2);
    PCMSK2 = (1 << PCINT20);

    
    state = PIND & PCMSK2;
}

/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= (1 << LED_PINNBR);

    DDRD &= ~(1<<PD4);
    PORTD |= (1<<PD4);



    sei(); /* Seta flag global de habilitação de interrupções */
}

/* -------------------------------------*/
volatile uint8_t has_changed;

ISR(PCINT2_vect)
{
    has_changed = 1;
}

/* -------------------------------------*/
int main(void)
{
    setup_pcint2();
    setup_main();

    uint8_t new_state, changed;

    PORTB |= (1<<LED_PINNBR);

    while(1){
    	if (has_changed==1){
    		new_state = PIND & PCMSK2;
  		    changed = new_state ^ state;
    		state = new_state;

    		if (changed & (1<<PD4)){
    			LED_PIN |= (1<<LED_PINNBR);
		    }
    		has_changed=0;
    	}  
    }

    return 0;
}
