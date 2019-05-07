#pragma once


#define DEBOUNCE_TIME_MS 50

typedef struct{
	virtual_timer_t vt_debounce;
	uint8_t IOPORT_N;
	uint8_t PIN_N;
	volatile bool last, cur, re, fe, re_aux, fe_aux, signal_debounced;
}deb_t;

void debounce_cb(void* par);