#include <stdlib.h>
#include <stdint.h>
#include <ch.h>
#include <hal.h>


icucnt_t width, old_width;
uint8_t flag;

uint16_t ciclo = 0;



//3 ponteiros p funcoes, overflow, etc...

//void echo_cb(ICUDriver *icup)
//{
//    width  = icuGetWidthX(&ICUD1); //guarda o width
//    flag = 1;
//}


void echo_pwm_cb(PWMDriver *pwmp)
{
    sdPut(&SD1, ciclo);
}


static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg)
{
    while (true) {
        palTogglePad(IOPORT2, PORTB_LED1);
        chThdSleepMilliseconds(400);
    }
}

int main(void)
{
    //static ICUConfig icucfg = {
    //    ICU_INPUT_ACTIVE_HIGH, 0, //borda de subida - ativação
    //    echo_cb, 0, 0   //ponteiros para as funcoes
    //};
    
    
    static PWMConfig pwmcfg = {
        0, 0, 0,   {{PWM_OUTPUT_ACTIVE_HIGH, NULL}, {PWM_OUTPUT_DISABLED, NULL}}
    };
    
    
    halInit();  //inicializa as paradas todas -- icuinit(), for example
    chSysInit();

    sdStart(&SD1, 0);
    //icuStart(&ICUD1, &icucfg);
    
    
    
    pwmStart(&PWMD1, &pwmcfg);

    
    
    
    palClearPad(IOPORT2, PORTB_LED1);

    
    
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    
    
    //DDRB |= (1<<4);
    //DDRB &= ~(1<<0);
    //uint16_t espera = 0;
    
    //palSetPadMode(IOPORT2,0,);
    
    DDRB |= (1<<1);
    PORTB &= ~(1<<1);
    
    while (1) {
    
        ciclo = sdGet(&SD1);
        if (ciclo > 100)
            ciclo = 100;
        
        sdPut(&SD1, ciclo);
        
        pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 80));
        
        chThdSleepMilliseconds(100);
       
    }

    return 0;
}
