#include <avr/io.h>
#include <avr/cpufunc.h>


volatile uint16_t intervalo1= 1000;
volatile uint16_t intervalo2= 300;


void delay_ms(uint16_t ms)
{
    uint16_t i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<4000; j++)
            _NOP();
}




uint8_t contar = 0;

int main(void)
{
   // DDRC |= _BV(DDC2);
    
    DDRB |= (1<<LED_PINNBR);
    DDRD |= (1<<PD7);
    
    while(1) {
        
        
        
        delay_ms(100);
        contar++;
        
        if ((contar*100)%intervalo2 == 0){
            
            PIND |= (1<<PD7);
            
        } 
        
        
        if ((contar*100)%intervalo1 == 0){
            
            PINB |= (1<<LED_PINNBR);
            
        } 
        
        
        
        
     /*   PORTD |= _BV(PD7);
        delay_ms(250);
        PORTB |= (1<<LED_PINNBR);
        delay_ms(250);
        
        
        PORTB &= ~(1<<LED_PINNBR);
        PORTD &= ~_BV(PD7);
        delay_ms(250);*/
    }

    return 0;
}
