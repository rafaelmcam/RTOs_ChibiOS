#include <stdlib.h>
#include <stdint.h>
#include <ch.h>
#include <hal.h>

#define TOL 2

icucnt_t width, old_width;
uint8_t flag;

//3 ponteiros p funcoes, overflow, etc...

void echo_cb(ICUDriver *icup)
{
    width  = icuGetWidthX(&ICUD1); //guarda o width
    flag = 1;
}

static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg)
{
    while (true) {
        palTogglePad(IOPORT2, PORTB_LED1);
        chThdSleepMilliseconds(100);
    }
}

int main(void)
{
    static ICUConfig icucfg = {
        ICU_INPUT_ACTIVE_HIGH, 0, //borda de subida - ativação
        echo_cb, 0, 0   //ponteiros para as funcoes
    };

    halInit();  //inicializa as paradas todas -- icuinit(), for example
    chSysInit();

    sdStart(&SD1, 0);
    icuStart(&ICUD1, &icucfg);

    palClearPad(IOPORT2, PORTB_LED1);

    
    
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    
    
    DDRB |= (1<<4);
    DDRB &= ~(1<<0);
    uint16_t espera = 0;
    
    //palSetPadMode(IOPORT2,0,);

    while (1) {
        palSetPad(IOPORT2, 4);
        
        for (espera = 0; espera<200; espera++);
        
        palClearPad(IOPORT2, 4);
       
    
        icuStartCapture(&ICUD1);
        while (!flag)
            chThdSleepMilliseconds(100);
        flag = 0;
        if (abs(width - old_width) < TOL) //verificar se a distancia é relevante
            continue;
        old_width = width;
        width = (width*34)>>5;
        sdPut(&SD1, width & 0xFF);
        sdPut(&SD1, (width >> 8) & 0xFF);
    }

    return 0;
}
