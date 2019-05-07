#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <hal_icu.h>
//#include <hal_icu_lld.h>

thread_t *main_th;

void echo_cb(ICUDriver *icup)
{
    ;
}

void dummy_cb(ICUDriver *icup)
{
}

static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg)
{
    while (true) {
        palTogglePad(IOPORT2, PORTB_LED1);
        chThdSleepMilliseconds(100);
    }
}

int main()
{
    static ICUConfig icucfg = {
        ICU_INPUT_ACTIVE_HIGH, 0,
        echo_cb, 0, dummy_cb
    };

    static ICUDriver ICUD = {
    	ICU_READY , &icucfg
    };

    halInit();
    chSysInit();

    sdStart(&SD1, NULL);
    icuInit();
    icuStart(&ICUD, &icucfg);

    palClearPad(IOPORT2, PORTB_LED1);

    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

    while (1) {
        icuStartCapture(&ICUD);
        //icuGetWidthX(&ICUD1);
    }

    return 0;
}