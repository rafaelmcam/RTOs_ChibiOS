#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#include "debouncer.h"


void debounce_cb(void *par)
{
	(*(deb_t*)par).re = false;
	(*(deb_t*)par).fe = false;

	switch(   (*(deb_t*)par).IOPORT_N    ){
		case 2:
			(*(deb_t*)par).cur = (palReadPad(IOPORT2, ((*(deb_t*)par).PIN_N))==PAL_HIGH);
			break;
		case 3:
			(*(deb_t*)par).cur = (palReadPad(IOPORT3, ((*(deb_t*)par).PIN_N))==PAL_HIGH);
			break;
		case 4:
			(*(deb_t*)par).cur = (palReadPad(IOPORT4, ((*(deb_t*)par).PIN_N))==PAL_HIGH);
			break;
		default:
			//não deveria acontecer se passar como parâmetro 2, 3 ou 4 (PORTB, PORTC, PORTD)
			break;
	}

	if ((*(deb_t*)par).last==false && (*(deb_t*)par).cur){
		//palTogglePad(IOPORT4, 7);
		(*(deb_t*)par).re = true;
		(*(deb_t*)par).re_aux = true;
		(*(deb_t*)par).last = true;
	}

	if ((*(deb_t*)par).last==true && !((*(deb_t*)par).cur)){
		//palTogglePad(IOPORT4, 7);
		(*(deb_t*)par).fe = true;
		(*(deb_t*)par).fe_aux = true;
		(*(deb_t*)par).last = false;
	}


	if ((*(deb_t*)par).re_aux==true && (*(deb_t*)par).fe_aux==false){
		(*(deb_t*)par).signal_debounced = true;
	}else if ((*(deb_t*)par).re_aux==true && (*(deb_t*)par).fe_aux==true){
		(*(deb_t*)par).re_aux = false;
		(*(deb_t*)par).fe_aux = false;
		(*(deb_t*)par).signal_debounced = false;
	}else{
		(*(deb_t*)par).signal_debounced = false;
	}

    chVTSet(&(*(deb_t*)par).vt_debounce, MS2ST(DEBOUNCE_TIME_MS), debounce_cb, par);
}