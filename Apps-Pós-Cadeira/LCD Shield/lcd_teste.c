#include <avr/io.h>
#include <avr/cpufunc.h>

#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#include "lcd.h"

#define VT_TIME_MS 1000

/*Para referenciar lcd.c e lcd.h ver no makefile a referência ao source SRC += $(CHIBIOS)/biblioteca_extra/lcd.c para o lcd.c e
EXTRAINCDIRS += $(CHIBIOS)/biblioteca_extra para lcd.h
*/

virtual_timer_t vt2;
int counter;

char str[4], str2[4];
uint16_t valor_geral=0;

void serial_cb(void *par)
{
    //chprintf((BaseSequentialStream *)&SD1, "%d\n", counter++);
    ADCSRA |= (1<<ADSC);
    valor_geral = ADC;

    lcd_clear();
    lcd_puts("TestandoBotaos:");
    lcd_set_cursor(0, 1);
    sprintf(str, "%d", counter++);
    lcd_puts(str);
    sprintf(str2, "%d", valor_geral);
    lcd_puts(str2);
    chVTSet(&vt2, MS2ST(VT_TIME_MS), serial_cb, 0);
}

static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg)
{
    while (true) {
        //nunca usar threads para botar as mensagens no LCD, usar o virtual timer como mostrado acima
		palTogglePad(IOPORT2, 5);
        chThdSleepMilliseconds(1000);
    }
}

int main(void)
{
    halInit();
    lcd_init();
    chSysInit();
    setup_adc_lcdshield_A0();


    DDRB |= (1 << 5);
    sdStart(&SD1, 0);
    palClearPad(IOPORT2, 5);
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);

    lcd_clear();
    lcd_puts("Iniciando LCD...");

    chVTSet(&vt2, MS2ST(VT_TIME_MS), serial_cb, 0);

    while (1) {
        ;
    }

    return 0;
}