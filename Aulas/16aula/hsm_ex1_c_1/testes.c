#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "event.h"
#include "sm.h"
#include "transitions3.h"

enum {

        EVENT_MOSTRAR = USER_EVENT,
        EVENT_CLEAR,
        EVENT_OPERADOR,
        EVENT_RESULTADO,
        EVENT_NUMERO

};

int num;
int num1, num2;
char operador;


cb_status init_cb(event_t ev)
{
        printf("top-INIT;");
        Topo_init_tran();
        return EVENT_HANDLED;
}


cb_status s_cb(event_t ev){

    switch (ev){
        case ENTRY_EVENT:
            printf("s-ENTRY;");
            return EVENT_HANDLED;
        case EXIT_EVENT:
            printf("s-EXIT;");
            return EVENT_HANDLED;
        case INIT_EVENT:
            printf("s-INIT;");
            s_init_tran();
            return EVENT_HANDLED;
        case EVENT_CLEAR:
            printf("s-CLEAR;");
            num1 = 0;
            num2 = 0;
            s_s_tran();
            return EVENT_HANDLED;
        case EVENT_MOSTRAR:
            printf("%d e %d--", num1, num2);
            return EVENT_HANDLED;
        case EVENT_RESULTADO:
        	printf("s-RESULTADO;");
        	return EVENT_HANDLED;
       	case EVENT_OPERADOR:
       		printf("s-OPERADOR;");
       		return EVENT_HANDLED;
    }

    return EVENT_NOT_HANDLED;
}

cb_status s1_cb(event_t ev){

    switch (ev){
            case ENTRY_EVENT:
                printf("s1-ENTRY;");
                return EVENT_HANDLED;
            case EXIT_EVENT:
                printf("s1-EXIT;");
                return EVENT_HANDLED;
            case INIT_EVENT:
                printf("s1-INIT;");
                return EVENT_HANDLED;
            case EVENT_NUMERO:
                printf("s1-NUMERO;");
                num1 = num;
              //  s1_s2_tran();
                return EVENT_HANDLED;
            case EVENT_OPERADOR:
            	printf("s1-OPERADOR;");
            	s1_s2_tran();
            	return EVENT_HANDLED;
    }
    return EVENT_NOT_HANDLED;
}


cb_status s2_cb(event_t ev){

    switch(ev){
            case ENTRY_EVENT:
                printf("s2-ENTRY;");
                return EVENT_HANDLED;
            case EXIT_EVENT:
                printf("s2-EXIT;");
                return EVENT_HANDLED;
            case INIT_EVENT:
                printf("s2-INIT;");
                return EVENT_HANDLED;
            case EVENT_NUMERO:
                printf("s2-NUMERO;");
                num2 = num;
               // s2_s1_tran();
                return EVENT_HANDLED;
            case EVENT_OPERADOR:
            	printf("s2-OPERADOR;");
            	return EVENT_HANDLED;
            case EVENT_RESULTADO:
            	printf("s2-RESULTADO;");
            	if (operador=='+')
            		printf("%d %c %d = %d", num1, operador, num2, num1+num2);
            	if (operador=='-')
            		printf("%d %c %d = %d", num1, operador, num2, num1-num2);
            	if (operador=='*')
            		printf("%d %c %d = %d", num1, operador, num2, num1*num2);
            	if (operador=='/')
            		printf("%d %c %d = %d", num1, operador, num2, num1/num2);
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
                        num = (int) (*(ptr-1)-'0');
                        set_event(EVENT_NUMERO);
                        break;
                    case 'C':
                    case 'c':
                        set_event(EVENT_CLEAR);
                        break;
                    case 'M':
                    case 'm':
                        set_event(EVENT_MOSTRAR);
                        break;
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    	set_event(EVENT_OPERADOR);
                    	operador = (*(ptr-1));
                    	break;
                   	case '=':
                   		set_event(EVENT_RESULTADO);
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


