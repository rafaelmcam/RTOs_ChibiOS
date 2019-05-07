#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include "event.h"
#include "sm.h"

cb_status s_cb(event_t ev);
cb_status s1_cb(event_t ev);
cb_status s2_cb(event_t ev);

#define Topo_init_tran() do {                   \
                push_state(s_cb);               \
                dispatch(INIT_EVENT);           \
        } while (0)


#define s_init_tran() do {                      \
                push_state(s1_cb);              \
                dispatch(ENTRY_EVENT);          \
        } while (0)


#define s_s_tran() do {                      \
                exit_inner_states();          \
                dispatch(EXIT_EVENT);        \
                dispatch(ENTRY_EVENT);       \
                dispatch(INIT_EVENT);        \
        } while (0)


#define s1_s2_tran() do {                      \
                exit_inner_states();          \
                dispatch(EXIT_EVENT);        \
                replace_state(s2_cb);          \
                dispatch(ENTRY_EVENT);        \
                dispatch(INIT_EVENT);          \
        } while (0)

#define s2_s1_tran() do {                      \
                exit_inner_states();          \
                dispatch(EXIT_EVENT);        \
                replace_state(s1_cb);          \
                dispatch(ENTRY_EVENT);        \
                dispatch(INIT_EVENT);          \
        } while (0)


#endif /* TRANSITIONS_H */
