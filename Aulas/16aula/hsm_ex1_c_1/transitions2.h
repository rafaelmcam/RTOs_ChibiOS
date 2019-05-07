#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include "event.h"
#include "sm.h"


cb_status init_cb(event_t ev);
cb_status ON_cb(event_t ev);
float calcular(float num1, float num2, char operador);
cb_status op1_cb(event_t ev);
float push_digito(float valor_anterior, int dig, int bol_ponto1, int* qnt_frac);
cb_status op1_inteiro_cb (event_t ev);
cb_status op2_cb(event_t ev);
cb_status op2_inteiro_cb (event_t ev);


#define Topo_init_tran() do {                   \
                push_state(ON_cb);              \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define on_init_tran() do {                     \
                push_state(op1_cb);             \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#define on_on_tran() do {                       \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)


#define op1_init_tran() do {                    \
                push_state(op1_inteiro_cb);     \
                dispatch(ENTRY_EVENT);          \
        } while (0)


#define op2_init_tran() do {                    \
                push_state(op2_inteiro_cb);     \
                dispatch(ENTRY_EVENT);          \
        } while (0)
        
        
#define op1_op2_tran() do {                     \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(op2_cb);          \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

        
#define op2_op1_tran() do {                     \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                replace_state(op1_cb);          \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)


#define op2_op2_tran() do {                     \
                exit_inner_states();            \
                dispatch(EXIT_EVENT);           \
                dispatch(ENTRY_EVENT);          \
                dispatch(INIT_EVENT);           \
        } while (0)

#endif /* TRANSITIONS_H */
 
