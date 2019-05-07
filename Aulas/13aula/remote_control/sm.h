#ifndef SM_H
#define SM_H

#include "event.h"

typedef void (*cb_t)(event_t ev);
typedef uint8_t state_t;

void dispatch_event(event_t ev);
void init_base_machine(cb_t *, uint8_t, cb_t, state_t);

extern state_t _state;
#define set_state(st) _state = st

#endif /* SM_H */
