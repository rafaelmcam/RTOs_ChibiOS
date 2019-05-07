#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>


#include <IRremote.h>

int RECV_PIN = 11; // define input pin on Arduino 
IRrecv irrecv(RECV_PIN); 
decode_results results; // decode_results class is defined in IRremote.h

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
    //chprintf((BaseSequentialStream *)&SD1, "%d\n", counter++);
    chVTSet(&vt2, S2ST(1), serial_cb, 0);
}

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD1, 0);

    palClearPad(LED_PORT, LED_PAD);

    irrecv.enableIRIn(); // Start the receiver 

    // digitalWrite(13, HIGH);

    chVTSet(&vt1, MS2ST(150), expiration_cb, 0);
    chVTSet(&vt2, S2ST(1), serial_cb, 0);
    while (1) {

        if (irrecv.decode(&results)) {
            //Serial.println(results.value, HEX); 
            chprintf((BaseSequentialStream *)&SD1, "%4x\n", results.value);
            irrecv.resume(); // Receive the next value 
        }
        chThdSleepMilliseconds(100);

    }

    return 0;
}
