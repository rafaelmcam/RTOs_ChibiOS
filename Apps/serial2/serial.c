#include <stdint.h>
#include <ch.h>
#include <hal.h>


// #define SERIAL_BITRATE 115200

// const SerialConfig sd_config = {
//   UBRR2x_F(SERIAL_BITRATE),
//   USART_CHAR_SIZE_8
// };

static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg)
{
    while (true) {
        palTogglePad(IOPORT2, PORTB_LED1);

        //PORTB ^= (1<<PB5);
        //PORTB ^= (1<<PORTB_LED1);

        chThdSleepMilliseconds(200);
    }
}

static THD_WORKING_AREA(waThread2, 16);
static THD_FUNCTION(Thread2, arg)
{
    while (true) {
        //palTogglePad(IOPORT2, PORTB_LED1);
        PORTB ^= (1<<PB0);
        chThdSleepMilliseconds(400);
    }
}

static THD_WORKING_AREA(waThread3, 16);
static THD_FUNCTION(Thread3, arg)
{
    while (true) {
        //palTogglePad(IOPORT2, PORTB_LED1);
        PORTB ^= (1<<PB1);
        chThdSleepMilliseconds(800);
    }
}

int main(void)
{
    uint16_t rc = 0;

    halInit();
    chSysInit();
    sdStart(&SD1, NULL);

    palClearPad(IOPORT2, PORTB_LED1);

    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    chThdCreateStatic(waThread2, sizeof(waThread1), NORMALPRIO, Thread2, NULL);
    chThdCreateStatic(waThread3, sizeof(waThread1), NORMALPRIO, Thread3, NULL);

    DDRB |= (1<<PB0);
   // PORTB |= (1<<PB0);

    DDRB |= (1<<PB1);
    //PORTB |= (1<<PB1);

    while(1) {
    	//chThdSleepMilliseconds(1000); se não tiver nada na thread principal pelo menos botar uma espera para funcionar
        rc = sdGet(&SD1);
        sdPut(&SD1, rc+1);
    }

    return 0;
}
