#include "sm.h"


state_t _active_states[MAX_ACTIVE_STATES];
state_t *_p_state = _active_states;
uint8_t _state_stack_len;


void init_machine(cb_t init_fun)
{
        *_active_states = 0;
        init_fun(INIT_EVENT);
}
