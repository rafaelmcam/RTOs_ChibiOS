#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include "event.h"
#include "sm.h"
#include "transitions2.h"


enum {
        EVENT_OPERADOR = USER_EVENT,
        EVENT_SINAL,
        EVENT_CLEAR,
        EVENT_PONTO,
        EVENT_NUMERO,
        EVENT_RESULT,
};


float result;
float num1, num2;
int bol_ponto_1 = 0, qnt_frac_1 = 1;
int bol_ponto_2 = 0, qnt_frac_2 = 1;

cb_status init_cb(event_t ev)
{
        result = 0;
        printf("top-INIT;");
        Topo_init_tran();
        return EVENT_HANDLED;
}

cb_status ON_cb(event_t ev)
{
        switch(ev) {
        case ENTRY_EVENT:
                printf("on-ENTRY;");
                return EVENT_HANDLED;
        case EXIT_EVENT:
                printf("on-EXIT;");
                return EVENT_HANDLED;
        case INIT_EVENT:
                printf("on-INIT;");
                on_init_tran();
                return EVENT_HANDLED;
        case EVENT_CLEAR:
                printf("on-CLEAR;");
                num1 = 0;
                num2 = 0;
                on_on_tran();
                return EVENT_HANDLED;
        default:
            printf("Algum Evento Não Foi Tratado por nenhum estado...\n");
            return EVENT_HANDLED;
        }

        return EVENT_NOT_HANDLED;
}


char operador, operador_anterior;
int num;



float calcular(float num1, float num2, char operador){
    
    float resultado = -103;
    
    switch(operador){
        
        case '+':
            resultado = num1 + num2;
            break;
        case '-':
            resultado = num1 - num2;
            break;
        case '*':
            resultado = num1 * num2;
            break;
        case '/':
            resultado = num1/num2;
            break;
        default:
            printf("Erro! Operador recebido: %c", operador);
            break;
        
    }
    
    
    return resultado;
}


cb_status op1_cb(event_t ev)
{
       switch(ev) {
        case ENTRY_EVENT:
                printf("op1-ENTRY;");
                return EVENT_HANDLED;
        case EXIT_EVENT:
                printf("op1-EXIT;");
                qnt_frac_1 = 1;
                bol_ponto_1 = 0;
                return EVENT_HANDLED;
        case INIT_EVENT:
                printf("op1-INIT;");
                op1_init_tran();
                return EVENT_HANDLED;
        case EVENT_OPERADOR:
                printf("op1-OPERADOR; '%c'", operador);
                op1_op2_tran();
                return EVENT_HANDLED;
        case EVENT_RESULT:  /* result -- "=" */
                printf("op1-RESULTADO;");
                return EVENT_HANDLED;
        }

        return EVENT_NOT_HANDLED;
}


float push_digito(float valor_anterior, int dig, int bol_ponto, int* qnt_frac){
    
    float operando;
    
    //printf("->>>>>>>>>>>>>%d", dig);
    if (bol_ponto==0){

    	if (dig==0){
    		//printf("RECEBI DIG=0 COM BOL_PONTO=0;");
    		operando = valor_anterior * 10;
    	}
    	else{

        operando = valor_anterior * 10 + (float)dig;
    	}


    }else{
        operando = valor_anterior + (float)dig/pow(10, (*qnt_frac)++);
    }
    return operando;
}

cb_status op1_inteiro_cb(event_t ev){
    
    
    switch(ev){
         case ENTRY_EVENT:
                printf("op1_inteiro-ENTRY;");
                return EVENT_HANDLED;
        case EXIT_EVENT:
                printf("op1_inteiro-EXIT;");
                return EVENT_HANDLED;
        case INIT_EVENT:
                printf("op1_init-INIT;");
              //  op1_inteiro_init_tran();
                return EVENT_HANDLED;
        case EVENT_NUMERO:
            num1 = push_digito(num1, num, bol_ponto_1, &qnt_frac_1);
            printf("op1_inteiro-NUMERO; Num1 atual: %f", num1);
            return EVENT_HANDLED;
        case EVENT_PONTO:
            printf("op1_inteiro-PONTO;");
            bol_ponto_1 = 1;
            return EVENT_HANDLED;
        case EVENT_SINAL:
            printf("op1_inteiro-SINAL;");
            num1 *= -1;
            return EVENT_HANDLED;
            
    }
    
    return EVENT_NOT_HANDLED;
    
}

cb_status op2_cb(event_t ev)
{
       switch(ev) {
        case ENTRY_EVENT:
                printf("op2-ENTRY;");
                return EVENT_HANDLED;
        case EXIT_EVENT:
                printf("op2-EXIT;");
                qnt_frac_2 = 1;
                bol_ponto_2 = 0;
                num2=0;
                return EVENT_HANDLED;
        case INIT_EVENT:
                printf("op2-INIT;");
                op2_init_tran();
                return EVENT_HANDLED;
        case EVENT_OPERADOR:
                printf("op2-OPERADOR; '%c'", operador);
                result = calcular(num1,num2,operador_anterior);
                num1 = result;
                printf("op2-OPERADOR: Visor mostrará: %f; ", result);
                op2_op2_tran();
                return EVENT_HANDLED;
        case EVENT_RESULT: 
                result = calcular(num1,num2,operador);
                num1 = result;
                op2_op2_tran();
                printf("op2-RESULTADO: Resultado da operação: %f", result);
                return EVENT_HANDLED;
        }

        return EVENT_NOT_HANDLED;
}



cb_status op2_inteiro_cb (event_t ev){
    
    
    switch(ev){
         case ENTRY_EVENT:
                printf("op2_inteiro-ENTRY;");
                return EVENT_HANDLED;
        case EXIT_EVENT:
                printf("op2_inteiro-EXIT;");
                return EVENT_HANDLED;
        case INIT_EVENT:
                printf("op2_init-INIT;");
             //   op2_inteiro_init_tran();
                return EVENT_HANDLED;
        case EVENT_NUMERO:
            num2 = push_digito(num2, num, bol_ponto_2, &qnt_frac_2);
            printf("op2_inteiro-NUMERO; Num2 atual: %f", num2);
            operador_anterior = operador;
            return EVENT_HANDLED;
        case EVENT_PONTO:
            printf("op2_inteiro-PONTO;");
            bol_ponto_2 = 1;
            return EVENT_HANDLED;
        case EVENT_SINAL:
            printf("op2_inteiro-SINAL;");
            num2 *= -1;
            return EVENT_HANDLED;

            
    }
    
    return EVENT_NOT_HANDLED;
    
}


pthread_mutex_t handling_mutex;
pthread_cond_t handling_cv;


void *event_thread(void *vargp)
{
        event_t ev;

        while (1) {
                ev = wait_for_events();
                if (ev == EMPTY_EVENT)
                        break;
                printf("\n");
                dispatch_event(ev);
                pthread_mutex_lock(&handling_mutex);
                pthread_cond_signal(&handling_cv);
                pthread_mutex_unlock(&handling_mutex);
        }

        return NULL;
}



int main(int argc, char* argv[])
{
        char *ptr;
        pthread_t tid;

        if (argc < 2) {
                fprintf(stderr, "Usage: %s inputs\n", argv[0]);
                return -1;
        }

        pthread_mutex_init(&handling_mutex, 0);
        pthread_cond_init(&handling_cv, 0);
        pthread_create(&tid, NULL, event_thread, NULL);

        init_machine(init_cb);
        ptr = argv[1];
        while(*ptr) {
                switch (*ptr++) {
                	case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    //    num = *ptr - '0';
                        num = *(ptr-1) - '0';
                    //    printf("NUM= %d;", num);
                        set_event(EVENT_NUMERO);
                        break;
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                        operador = *(ptr-1);
                        set_event(EVENT_OPERADOR);
                        break;
                    case 'C':
                    case 'c':
                        set_event(EVENT_CLEAR);
                        break;
                    case '.':
                        set_event(EVENT_PONTO);
                        break;
                    case '=':
                        set_event(EVENT_RESULT);
                        break;
                    case 'S':
                    case 's':
                        set_event(EVENT_SINAL);
                        break;
                    default:
                        continue;

                }
                pthread_mutex_lock(&handling_mutex);
                pthread_cond_wait(&handling_cv, &handling_mutex);
                pthread_mutex_unlock(&handling_mutex);
        }
        printf("\n");

        set_event(EMPTY_EVENT);
        pthread_join(tid, NULL);

        return 0;
}


