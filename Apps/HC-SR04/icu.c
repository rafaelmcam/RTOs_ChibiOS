
#include <stdlib.h>
#include <stdint.h>
#include <ch.h>
#include <hal.h>


#include <chprintf.h>


#define TOL 2

icucnt_t width, old_width;
uint8_t flag;
uint32_t atraso = 100;

uint16_t espera = 0;
virtual_timer_t vt1;
volatile int counter;


void expiration_cb(void *par)
{
   

   	//botar trigger artificial aqui mas o virtual timer está interferindo na thread.

   	chprintf((BaseSequentialStream *)&SD1, "%d\n", counter++);
    flag =1;
    
}



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

static THD_WORKING_AREA(waThread2, 32);
static THD_FUNCTION(Thread2, arg)
{
    while (true) {


    	palSetPad(IOPORT2, 4);

    	chVTSet(&vt1, MS2ST(100), expiration_cb, 0);

        for (espera = 0; espera<25; espera++);
        palClearPad(IOPORT2, 4);
       
    
        icuStartCapture(&ICUD1);
        while (!flag)
            chThdSleepMilliseconds(50);
        flag = 0;
        if (abs(width - old_width) < TOL) //verificar se a distancia é relevante
            continue;
        old_width = width;
        width = (width*34)>>5;

        atraso = (width*10)/(34);

        if (atraso > 1000){

         atraso = 1000;

        }
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

    
    
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);
    chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO + 2, Thread2, NULL);
    
    DDRB |= (1<<4);
    DDRB &= ~(1<<0);
  
    


    DDRB |= (1<<3);

    //palSetPadMode(IOPORT2,0,);

    //chVTSet(&vt1, MS2ST(1000), expiration_cb, 0);

    while (1) {

    	;
    }

    return 0;
}
