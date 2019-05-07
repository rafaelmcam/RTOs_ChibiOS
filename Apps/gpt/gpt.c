#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#define LED_PAD PORTB_LED1
#define LED_PORT IOPORT2

volatile uint8_t flag;

void expiration_cb(GPTDriver *gptp)
{
    flag = 1;
}

static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg)
{
    while (true) {
	palTogglePad(LED_PORT, LED_PAD);
        chThdSleepMilliseconds(500);
    }
}

int main(void)
{
    uint16_t counter = 0;

    static GPTConfig gptcfg = {1000, expiration_cb};

    halInit();
    chSysInit();

    sdStart(&SD1, 0);
    gptStart(&GPTD1, &gptcfg);

    palClearPad(LED_PORT, LED_PAD);

    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);

    gptStartContinuous(&GPTD1, 1000);
    while (1) {
        while(!flag) {}
        flag = 0;
        chprintf((BaseSequentialStream *)&SD1, "Contador: %u\n", counter++);
    }

    return 0;
}
