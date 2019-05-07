#include "timer.h"
#include "usart.h"
#include "event.h"
#include "sm.h"
#include "sm_decoder.h"
#include "debug.h"


void write_byte(uint8_t n)
{
	uint8_t tmp;

	tmp = (n >> 4) & 0xF;
	write(tmp + (tmp > 9 ? 'A' - 10 : '0'));
	tmp =  n       & 0xF;
	write(tmp + (tmp > 9 ? 'A' - 10 : '0'));
}


void write_word(uint16_t n)
{
	write_byte(n >> 8);
	write_byte(n);
}


enum {S1, S2, S3, S4, MAX_STATES};

uint8_t _bytes[5];
uint8_t _idx_byte, _idx_bit;

void push_bit(uint8_t bit){
	if (bit)
		_bytes[_idx_byte] |= _BV(_idx_bit);
	if (++_idx_bit >= 8) {
		_idx_byte++;
		_idx_bit = 0;
	}
}

void send_bytes(void)
{
	int8_t i;

	for(i=sizeof(_bytes)-1; i>=0; i--) {
		write_byte(_bytes[i]);
		_bytes[i] = 0;
	}
	_idx_byte = _idx_bit = 0;
}

void f1(event_t ev)
{
	if (ev == EVENT1) {
		DEBUG_WRITE_STATE("S1E1");
		set_state(S2);
	} else if (ev == EVENT7)
		;
	else
		DEBUG_WRITE_STATE_EVENT("S1E*", ev);
}

void f2(event_t ev)
{
	if (ev == EVENT2) {
		DEBUG_WRITE_STATE("S2E2");
		set_state(S3);
	} else {
		DEBUG_WRITE_STATE_EVENT("S2E* ", ev);
		set_state(S1);
	}
}

void f3(event_t ev)
{
	if (ev == EVENT3) {
		DEBUG_WRITE_STATE("S3E3");
		set_state(S4);
	} else {
		DEBUG_WRITE_STATE_EVENT("S3E*", ev);
		set_state(S1);
	}
}

void f4(event_t ev)
{
	if (ev == EVENT4) {
		DEBUG_WRITE_STATE("S4E4");
		push_bit(1);
		set_state(S3);
	} else if (ev == EVENT5) {
		DEBUG_WRITE_STATE("S4E5");
		push_bit(0);
		set_state(S3);
	} else if (ev == EVENT6) {
		DEBUG_WRITE_STATE("S4E6");
		set_state(S3);
	} else if (ev == EVENT7) {
		DEBUG_WRITE_STATE("S4E7");
		set_state(S1);
		send_bytes();
		write('\n');
	} else {
		DEBUG_WRITE_STATE_EVENT("S4E*", ev);
		set_state(S1);
	}
}

cb_t cb_array[MAX_STATES] = {f1, f2, f3, f4};

void init_machine(void)
{
	init_base_machine(cb_array, MAX_STATES, 0, S1);
	setup_usart();
	setup_timer1_timer();
}
