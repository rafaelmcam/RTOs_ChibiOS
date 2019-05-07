#include "sm.h"


cb_t *_cb_array;
cb_t _initial_transition;
uint8_t _cb_len;
state_t _initial_state;
state_t _state;


void init_base_machine(cb_t *cbs, uint8_t len, cb_t initial_transition, state_t initial_state)
{
	_initial_transition = initial_transition;
	_state = _initial_state = initial_state;
	_cb_len = len;
	_cb_array = cbs;

	if (initial_transition)
		initial_transition(0);
}


void dispatch_event(event_t ev)
{
	_cb_array[_state](ev);
}
