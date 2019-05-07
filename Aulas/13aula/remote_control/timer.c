#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "event.h"

#define FOSC 16000000
/* Nbr of timer clock pulses per ms assuming a prescaler factor of 8 */
#define FTIM FOSC/8/1000

#define MS1L  3.0*FTIM
#define MS1H 14.0*FTIM
#define MS2L  2.5*FTIM
#define MS2H  6.0*FTIM
#define MS3L  0.4*FTIM
#define MS3H  2.0*FTIM
#define MS4L  1.05*FTIM
#define MS4H  2.0*FTIM
#define MS5L  0.2*FTIM
#define MS5H  1.0*FTIM
#define MS6L 12.0*FTIM
#define MS6H 28.0*FTIM


/* -------------------------------------*/
void setup_timer1_timer(void)
{
	TCCR1A = 0;
	OCR1A = 0xFFFF;
	TCCR1B |= _BV(CS11) | _BV(WGM12); // CLOCK/8, CTC mode
	TIMSK1 |= _BV(ICIE1) | _BV(OCIE1A);
}

/* -------------------------------------*/
uint8_t _later_edge;
uint8_t _flag;
uint16_t _t1, _t2;

ISR(TIMER1_CAPT_vect)
{
	uint16_t delta;
	uint8_t tmp;
	uint32_t big;

	_t2 = ICR1;
	_flag = 1;
	if (!_later_edge) {
		_later_edge = 1;
		_t1 = _t2;
		TCCR1B ^= _BV(ICES1);
		return;
	}
	if (_t2 < _t1) {
		big = (1l << 16) + _t2 - _t1;
		delta = big;
	}
	else
		delta = _t2 - _t1;
	_t1 = _t2;

	tmp = TCCR1B;
	if (tmp & _BV(ICES1)) { /* Borda de subida, intervalo off */
		TCCR1B = tmp & ~_BV(ICES1);

		if ((delta < MS1H) && (delta > MS1L))
			set_event(EVENT1);
		else if ((delta < MS3H) && (delta > MS3L))
			set_event(EVENT3);
	} else { /* Borda de descida, intervalo on */
		TCCR1B = tmp | _BV(ICES1);

		if ((delta < MS2H) && (delta > MS2L))
			set_event(EVENT2);
		else if ((delta < MS4H) && (delta > MS4L))
			set_event(EVENT4);
		else if ((delta < MS5H) && (delta > MS5L))
			set_event(EVENT5);
		else if ((delta < MS6H) && (delta > MS6L))
			set_event(EVENT6);
	}
}


/* -------------------------------------*/
ISR(TIMER1_COMPA_vect)
{
	if (!_flag) {
		TCCR1B &= ~_BV(ICES1);
		_later_edge = 0;
		set_event(EVENT7);
	}
	_flag = 0;
}
