#include <avr/io.h>
#include <avr/cpufunc.h>

void delay_ms(uint16_t ms)
{
    uint16_t i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<4000; j++)
            _NOP();
}

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
    LED_DDR |= (1 << LED_PIN);
    while(1) {
        LED_PORT |= (1 << LED_PIN);
        delay_ms(250);

        LED_PORT &= ~(1 << LED_PIN);
        delay_ms(1000);
    }

    return 0;
}
