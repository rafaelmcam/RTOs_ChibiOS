#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include "event.h"
#include "sm.h"


cb_status s_cb(event_t ev);
cb_status s1_cb(event_t ev);
cb_status s11_cb(event_t ev);
cb_status s2_cb(event_t ev);
cb_status s21_cb(event_t ev);
cb_status s211_cb(event_t ev);


#define Topo_init_tran() do {                   \
                push_state(on_cb);               \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define on_init_tran() do {                      \
                push_state(op1_cb);              \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define on_on_tran() do {                      \
                exit_inner_states();              \
                dispatch(EXIT_EVENT);              \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)
        
        
#define op1_op2_tran() do {                       \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(op2_cb);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

        
#define op2_op1_tran() do {                       \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(op1_cb);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)
        

#endif /* TRANSITIONS_H */
 
