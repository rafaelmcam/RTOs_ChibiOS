#include <avr/io.h>
#include <avr/cpufunc.h>

void delay_ms(uint16_t ms)
{
    uint16_t i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<4000; j++)
            _NOP();
}

int main(void)
{
    DDRC |= _BV(DDC2);
    while(1) {
        PORTC |= _BV(DDC2);
        delay_ms(250);

        PORTC &= ~_BV(DDC2));
        delay_ms(250);
    }

    return 0;
}
