#ifndef SM_H
#define SM_H

#include <stdint.h>
#include "event.h"

typedef uint8_t cb_status;
enum {
        EVENT_HANDLED = 0,
        EVENT_NOT_HANDLED
};

enum {
        EMPTY_EVENT = EVENT0,
        ENTRY_EVENT = EVENT1,
        EXIT_EVENT  = EVENT2,
        INIT_EVENT  = EVENT3,
        USER_EVENT  = EVENT4
};

typedef cb_status (*cb_t)(event_t ev);
typedef cb_t state_t;

#define MAX_ACTIVE_STATES 10
extern state_t *_p_state;
extern uint8_t _state_stack_len;

void init_machine(cb_t init_fun);

#define dispatch(ev) (*_p_state)(ev)
#define push_state(st) *++_p_state = (st)
#define pop_state() _p_state--
#define replace_state(st) *_p_state = (st)
#define exit_inner_states()                     \
        do {                                    \
                _p_state += _state_stack_len;   \
                while (_state_stack_len) {      \
                        dispatch(EXIT_EVENT);   \
                        pop_state();            \
                        _state_stack_len--;     \
                }                               \
        } while(0)

#define dispatch_event(ev) do {                                 \
        _state_stack_len = 0;                                   \
        while(*_p_state) {                                      \
                if (dispatch(ev) == EVENT_HANDLED) {            \
                        _p_state += _state_stack_len;           \
                        break;                                  \
                }                                               \
                _state_stack_len++;                             \
                pop_state();                                    \
        }                                                       \
        if (!*_p_state) {                                       \
                _p_state += _state_stack_len;                   \
                _state_stack_len = 0;                           \
        }                                                       \
        } while (0)

#endif /* SM_H */
