#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#include "debouncer.h"

#define LED_PORT IOPORT2
#define LED_PAD 5

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD1, 0);

    palClearPad(LED_PORT, LED_PAD);

    palSetPadMode(IOPORT2, 2, PAL_MODE_INPUT);
    palSetPadMode(IOPORT2, 3, PAL_MODE_INPUT);
    
    deb_t sinal_deb = {.IOPORT_N = 2, .PIN_N = 2};
    deb_t sinal_deb2 = {.IOPORT_N = 2, .PIN_N = 3};

    void* aux = &sinal_deb;
    void* aux2 = &sinal_deb2;

    
    chVTSet(&sinal_deb.vt_debounce, MS2ST(DEBOUNCE_TIME_MS), debounce_cb, aux);
    chVTSet(&sinal_deb2.vt_debounce, MS2ST(DEBOUNCE_TIME_MS), debounce_cb, aux2);

    bool last = false;
    bool last2 = false;

    while (1) {

    	// if (last2 == false && sinal_deb2.signal_debounced == true){
    	// 	palTogglePad(IOPORT4, 7);
    	// 	last2 = true;
    	// }

    	// if (last2 == true && sinal_deb2.signal_debounced == false){
    	// 	last2 = false;
    	// }



    	if (last == false && sinal_deb.signal_debounced == true){
    		palTogglePad(IOPORT4, 7);
    		last = true;
    	}

    	if (last == true && sinal_deb.signal_debounced == false){
    		last = false;
    	}

    	//mostrar que a mesma entrada com e sem o debouncer.c, com fica togglando uma vez por aperto e sem as vezes inverte mais de 1 vez
    	if (last2 == false && palReadPad(IOPORT2, 2) == true){
    		palTogglePad(IOPORT4, 6);
    		last2 = true;
    	}

    	if (last2 == true && palReadPad(IOPORT2, 2) == false){
    		last2 = false;
    	}
    	
    }

    return 0;
}
