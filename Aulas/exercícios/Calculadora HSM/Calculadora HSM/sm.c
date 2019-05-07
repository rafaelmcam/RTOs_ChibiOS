#include "sm.h"


state_t _active_states[MAX_ACTIVE_STATES] = {0};
state_t *_p_state = _active_states;
uint8_t _state_stack_len;
