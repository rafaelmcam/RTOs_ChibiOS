#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#define LED_PAD PORTB_LED1
#define LED_PORT IOPORT2

virtual_timer_t vt1, vt2;
int counter;

void expiration_cb(void *par)
{
    palTogglePad(LED_PORT, LED_PAD);
    chVTSet(&vt1, MS2ST(150), expiration_cb, 0);
}

void serial_cb(void *par)
{
    chprintf((BaseSequentialStream *)&SD1, "%d\n", counter++);
    chVTSet(&vt2, S2ST(1), serial_cb, 0);
}

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD1, 0);

    palClearPad(LED_PORT, LED_PAD);

    chVTSet(&vt1, MS2ST(150), expiration_cb, 0);
    chVTSet(&vt2, S2ST(1), serial_cb, 0);
    while (1) {}

    return 0;
}
