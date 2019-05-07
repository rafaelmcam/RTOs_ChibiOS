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
                push_state(s_cb);               \
                dispatch(ENTRY_EVENT);          \
                push_state(s2_cb);              \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define s_init_tran() do {                      \
                push_state(s1_cb);              \
                dispatch(ENTRY_EVENT);          \
                push_state(s11_cb);             \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define s_s11_local_tran() do {                 \
                exit_inner_states();            \
                push_state(s1_cb);              \
                dispatch(ENTRY_EVENT);          \
                push_state(s11_cb);             \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define s1_init_tran() do {                     \
                push_state(s11_cb);             \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define s1_s_local_tran() do {                  \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(INIT_EVENT);           \
        } while (0)

#define s1_s1_tran() do {                       \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define s1_s11_local_tran() do {                \
                exit_inner_states();            \
                push_state(s11_cb);             \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define s1_s2_tran() do {                       \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(s2_cb);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define s1_s211_tran() do {                     \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(s2_cb);           \
                dispatch(ENTRY_EVENT);          \
                push_state(s21_cb);             \
                dispatch(ENTRY_EVENT);          \
                push_state(s211_cb);            \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define s11_s_local_tran() do {                 \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(INIT_EVENT);           \
        } while(0)

#define s11_s1_local_tran() do {                \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(INIT_EVENT);           \
        } while(0)

#define s11_s211_tran() do {                    \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(EXIT_EVENT);           \
                replace_state(s2_cb);           \
                dispatch(ENTRY_EVENT);          \
                push_state(s21_cb);             \
                dispatch(ENTRY_EVENT);          \
                push_state(s211_cb);            \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define s2_init_tran() do {                     \
                push_state(s21_cb);             \
                dispatch(ENTRY_EVENT);          \
                push_state(s211_cb);            \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define s2_s1_tran() do {                       \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(s1_cb);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while(0)

#define s2_s11_tran() do {                      \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(s1_cb);           \
                dispatch(ENTRY_EVENT);          \
                push_state(s11_cb);             \
                dispatch(ENTRY_EVENT);          \
        } while(0)

#define s21_init_tran() do {                    \
                push_state(s211_cb);            \
                dispatch(ENTRY_EVENT);          \
        } while (0)

#define s21_s21_tran() do {                     \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while(0)

#define s21_s211_local_tran() do {              \
                exit_inner_states();            \
                push_state(s211_cb);            \
                dispatch(ENTRY_EVENT);          \
        } while(0)

#define s21_s11_tran() do {                     \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(EXIT_EVENT);           \
                replace_state(s1_cb);           \
                dispatch(ENTRY_EVENT);          \
                push_state(s11_cb);             \
                dispatch(ENTRY_EVENT);          \
        } while(0)

#define s211_s_local_tran() do {                \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(INIT_EVENT);           \
        } while(0)

#define s211_s21_local_tran() do {              \
                dispatch(EXIT_EVENT);           \
                pop_state();                    \
                dispatch(INIT_EVENT);           \
        } while(0)

#endif /* TRANSITIONS_H */
