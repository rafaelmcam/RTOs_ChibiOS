#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include "event.h"
#include "sm.h"


cb_status g_cb(event_t ev);
cb_status a1_cb(event_t ev);
cb_status a2_cb(event_t ev);

cb_status gvd1a_cb(event_t ev);
cb_status gvd1b_cb(event_t ev);
cb_status gam1a_cb(event_t ev);
cb_status gam1b_cb(event_t ev);
cb_status gvd2_cb(event_t ev);
cb_status gam2_cb(event_t ev);
cb_status gvd3_cb(event_t ev);
cb_status gam3a_cb(event_t ev);
cb_status gam3b_cb(event_t ev);

#define Topo_init_tran() do {                   \
                push_state(g_cb);               \
                dispatch(ENTRY_EVENT);          \
                push_state(gvd1a_cb);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)


#define gvd1a_gvd1b_tran() do {                 \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gvd1b_cb);        \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define gvd1b_gam1a_tran() do {                 \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gam1a_cb);        \
                dispatch(ENTRY_EVENT);          \
        } while (0)


#define gvd1b_gam1b_tran() do {                 \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gam1b_cb);        \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define gam1a_gvd2_tran() do {                  \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gvd2_cb);         \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define gvd2_gam2_tran() do {                   \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gam2_cb);         \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define gam2_gvd1a_tran() do {                  \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gvd1a_cb);        \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define gam1b_gvd3_tran() do {                  \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gvd3_cb);         \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define gvd3_gam3a_tran() do {                  \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gam3a_cb);        \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define gam3a_gvd1a_tran() do {                 \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gvd1a_cb);        \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define gvd3_gam3b_tran() do {                  \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gam3b_cb);        \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define gam3b_gvd2_tran() do {                  \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(gvd2_cb);         \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define g_a1_tran() do {                        \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(a1_cb);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define a1_gvd1b_tran() do {                    \
        	exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(g_cb);            \
                dispatch(ENTRY_EVENT);          \
                push_state(gvd1b_cb);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define g_a2_tran() do {                        \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(a2_cb);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define a2_gam2_tran() do {                     \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(g_cb);            \
                dispatch(ENTRY_EVENT);          \
                push_state(gam2_cb);            \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#endif /* TRANSITIONS_H */
