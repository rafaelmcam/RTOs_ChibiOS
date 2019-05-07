
/*

Projeto de RTOs - UFPE 2017.2

Projeto Escolhido: Semáforos

Grupo: 

Diógenes Wallis
João Vicente
Rafael Campello

*/

#include <stdio.h>
#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include "event.h"
#include "sm.h"
#include "transitions.h"

#define LED_PAD PORTB_LED1
#define LED_PORT IOPORT2

// Tempos definidos nos slides, aqui foram reduzidos para economizar tempo
#define GVD1a_TIMEOUT 6
#define GVD2_TIMEOUT 5
#define GVD3_TIMEOUT 4
#define AMARELO_TIMEOUT 3
#define PERIODO_PISCAR_MS 300
#define TEMPO_MINIMO_VERMELHO_AMBULANCIA 1

int timeout;

/*Definidos dois Virtual Timers, o primeiro para auxiliar na debugação do projeto, imprime a cada segundo
os valores dos contadores + as duas flags (carro2avia e pedestre), o segundo é utilizado por cada estado
para gerar um evento de TIMEOUT quando o valor correspondente a cada estado definido acima ocorrer.
*/

virtual_timer_t vt_aux;
volatile int counter_aux;

virtual_timer_t vt_geral;
volatile int counter_geral = 0;

uint8_t flag_pedestre = 0, flag_carro2avia = 0;
volatile uint8_t piscar = 0;

uint8_t flag_ambu_principal = 0, flag_ambu_2avia = 0;

void* aux = &timeout;

enum {
        EVENT_A = USER_EVENT,
        EVENT_TIMEOUT,
        EVENT_CARRO_2VIA,
        EVENT_PEDESTRE,
        EVENT_AMBULANCIA_PRINCIPAL,
        EVENT_AMBULANCIA_PRINCIPAL_PASSOU,
        EVENT_AMBULANCIA_2VIA,
        EVENT_AMBULANCIA_2VIA_PASSOU,
        EVENT_H,
        EVENT_I
};

void contador_aux_cb(void *par) //contador utilizado para setar eventos na debugação
{
    chprintf((BaseSequentialStream *)&SD1, "%d-%d-%d-%d\n", counter_aux++, counter_geral,flag_carro2avia, flag_pedestre);
    chVTSet(&vt_aux, S2ST(1), contador_aux_cb, 0);
}

void contador_geral_cb(void *par)
{
    if (counter_geral < *(int*)par - 1){
    	counter_geral++;
    	chVTSet(&vt_geral, S2ST(1), contador_geral_cb, par);
    }else{
    	//counter_geral = 0;
    	set_event(EVENT_TIMEOUT);
    }
}

//Ao entrar em cada estado essa função é chamada para definir quais leds estão acesas no momento
void set_semaforos(int configuracao){
	switch(configuracao){
		case 0: //gvd1a, gvd1b
			palClearPad(IOPORT2, 1); //vermelho-principal
			palClearPad(IOPORT2, 2); //amarelo-principal
			palSetPad(IOPORT2, 3);  //verde-principal

			palSetPad(IOPORT3, 4);  //vermelho-pedestre
			palClearPad(IOPORT3, 5); //verde-pedestre 

			palSetPad(IOPORT4, 2); //vermelho-2a_via
			palClearPad(IOPORT4, 3); //amarelo-2a_via
			palClearPad(IOPORT4, 4); //verde-2a_via
			break;
		case 1: //gam1a,gam1b
			palClearPad(IOPORT2, 1); //vermelho-principal
			palSetPad(IOPORT2, 2); //amarelo-principal
			palClearPad(IOPORT2, 3);  //verde-principal

			palSetPad(IOPORT3, 4);  //vermelho-pedestre
			palClearPad(IOPORT3, 5); //verde-pedestre 

			palSetPad(IOPORT4, 2); //vermelho-2a_via
			palClearPad(IOPORT4, 3); //amarelo-2a_via
			palClearPad(IOPORT4, 4); //verde-2a_via
			break;
		case 2: //gvd2
			palSetPad(IOPORT2, 1); //vermelho-principal
			palClearPad(IOPORT2, 2); //amarelo-principal
			palClearPad(IOPORT2, 3);  //verde-principal

			palSetPad(IOPORT3, 4);  //vermelho-pedestre
			palClearPad(IOPORT3, 5); //verde-pedestre 

			palClearPad(IOPORT4, 2); //vermelho-2a_via
			palClearPad(IOPORT4, 3); //amarelo-2a_via
			palSetPad(IOPORT4, 4); //verde-2a_via
			break;
		case 3: //gam2
			palSetPad(IOPORT2, 1); //vermelho-principal
			palClearPad(IOPORT2, 2); //amarelo-principal
			palClearPad(IOPORT2, 3);  //verde-principal

			palSetPad(IOPORT3, 4);  //vermelho-pedestre
			palClearPad(IOPORT3, 5); //verde-pedestre 

			palClearPad(IOPORT4, 2); //vermelho-2a_via
			palSetPad(IOPORT4, 3); //amarelo-2a_via
			palClearPad(IOPORT4, 4); //verde-2a_via
			break;
		case 4: //gvd3
			palSetPad(IOPORT2, 1); //vermelho-principal
			palClearPad(IOPORT2, 2); //amarelo-principal
			palClearPad(IOPORT2, 3);  //verde-principal

			palClearPad(IOPORT3, 4);  //vermelho-pedestre
			palSetPad(IOPORT3, 5); //verde-pedestre 

			palSetPad(IOPORT4, 2); //vermelho-2a_via
			palClearPad(IOPORT4, 3); //amarelo-2a_via
			palClearPad(IOPORT4, 4); //verde-2a_via
			break;
		case 5: //gam3a, gam3b POR ENQUANTO DEIXAR VERMELHO MAS VAI TER QUE PISCAR
			palSetPad(IOPORT2, 1); //vermelho-principal
			palClearPad(IOPORT2, 2); //amarelo-principal
			palClearPad(IOPORT2, 3);  //verde-principal

			palSetPad(IOPORT3, 4);  //vermelho-pedestre
			palClearPad(IOPORT3, 5); //verde-pedestre 

			palSetPad(IOPORT4, 2); //vermelho-2a_via
			palClearPad(IOPORT4, 3); //amarelo-2a_via
			palClearPad(IOPORT4, 4); //verde-2a_via
			piscar = 1;
			break;
	}
	return;
}

cb_status init_cb(event_t ev)
{
        chprintf((BaseSequentialStream *)&SD1, "top-INIT;");
        Topo_init_tran();
        return EVENT_HANDLED;
}

//Estados definidos na máquina de estados enviado em anexo.
//No evento ENTRY de cada estado interno (ex: gvd1a) chamamos o contador que setará o EVENT_TIMEOUT correspondente
//No macroestado g associamos os eventos de pedestre e carro2avia à flags

cb_status g_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gEN;");
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gEX;");
			return EVENT_HANDLED;
		case INIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gIN;");
			return EVENT_HANDLED;
		case EVENT_PEDESTRE:
			flag_pedestre = 1;
			return EVENT_HANDLED;
		case EVENT_CARRO_2VIA:
			flag_carro2avia = 1;
			return EVENT_HANDLED;
		case EVENT_AMBULANCIA_PRINCIPAL:
			g_a1_tran();
			return EVENT_HANDLED;
		case EVENT_AMBULANCIA_2VIA:
			g_a2_tran();
			return EVENT_HANDLED;
		default:
			//chprintf((BaseSequentialStream *)&SD1, "g-ev não tratado");
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status a1_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "a1EN;");
			chVTReset(&vt_geral);
			counter_geral = 0;
			set_semaforos(0);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "a1EX;");
			return EVENT_HANDLED;
		case EVENT_PEDESTRE:
			flag_pedestre = 1;
			return EVENT_HANDLED;
		case EVENT_CARRO_2VIA:
			flag_carro2avia = 1;
			return EVENT_HANDLED;
		case EVENT_AMBULANCIA_PRINCIPAL_PASSOU:
			a1_gvd1b_tran();
			return EVENT_HANDLED;
		case INIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "a1IN;");
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			chprintf((BaseSequentialStream *)&SD1, "tmout vt;");
			return EVENT_HANDLED;
		default:
			chprintf((BaseSequentialStream *)&SD1, "a1-ev não tratado;");
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status a2_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "a2EN;");
			chVTReset(&vt_geral);
			counter_geral = 0;
			set_semaforos(2);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "a2EX;");
			return EVENT_HANDLED;
		case INIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "a2IN;");
			return EVENT_HANDLED;
		case EVENT_PEDESTRE:
			flag_pedestre = 1;
			return EVENT_HANDLED;
		case EVENT_CARRO_2VIA:
			flag_carro2avia = 1;
			return EVENT_HANDLED;
		case EVENT_AMBULANCIA_2VIA_PASSOU:
			a2_gam2_tran();
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			chprintf((BaseSequentialStream *)&SD1, "tmout vt;");
			return EVENT_HANDLED;
		default:
			chprintf((BaseSequentialStream *)&SD1, "a2-ev não tratado;");
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}


cb_status gvd1a_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd1aEN;");
			timeout = GVD1a_TIMEOUT;
			counter_geral = 0;
			chVTSet(&vt_geral, S2ST(1), contador_geral_cb, aux);
			set_semaforos(0);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd1aEX;");
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			gvd1a_gvd1b_tran();
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status gvd1b_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd1bEN;");
			/*Como este estado não tem timeout, definimos o valor do counter_geral como maior que o tempo minimo para que a ambulancia possa
			setar o evento.*/
			counter_geral = TEMPO_MINIMO_VERMELHO_AMBULANCIA + 1;
			set_semaforos(0);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd1bEX;");
			return EVENT_HANDLED;
		case INIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd1bIN;");
			if (flag_pedestre == 1){
				gvd1b_gam1b_tran();
			}else if(flag_carro2avia == 1){
				gvd1b_gam1a_tran();
			}
			return EVENT_HANDLED;
		case EVENT_PEDESTRE:
			gvd1b_gam1b_tran();
			return EVENT_HANDLED;
		case EVENT_CARRO_2VIA:
			gvd1b_gam1a_tran();
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status gam1a_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam1aEN;");
			counter_geral = 0;
			timeout = AMARELO_TIMEOUT;
			chVTSet(&vt_geral, S2ST(1), contador_geral_cb, aux);
			set_semaforos(1);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam1aEX;");
			flag_carro2avia = 0;
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			gam1a_gvd2_tran();
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status gam1b_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam1bEN;");
			counter_geral = 0;
			timeout = AMARELO_TIMEOUT;
			chVTSet(&vt_geral, S2ST(1), contador_geral_cb, aux);
			set_semaforos(1);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam1bEX;");
			flag_pedestre = 0;
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			gam1b_gvd3_tran();
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status gvd2_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd2EN;");
			counter_geral = 0;
			timeout = GVD2_TIMEOUT;
			chVTSet(&vt_geral, S2ST(1), contador_geral_cb, aux);
			set_semaforos(2);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd2EX;");
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			gvd2_gam2_tran();
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status gam2_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam2EN;");
			counter_geral = 0;
			timeout = AMARELO_TIMEOUT;
			chVTSet(&vt_geral, S2ST(1), contador_geral_cb, aux);
			set_semaforos(3);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam2EX;");
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			gam2_gvd1a_tran();
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status gvd3_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd3EN;");
			counter_geral = 0;
			timeout = GVD3_TIMEOUT;
			chVTSet(&vt_geral, S2ST(1), contador_geral_cb, aux);
			set_semaforos(4);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gvd3EX;");
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			if (flag_carro2avia == 0){
				gvd3_gam3a_tran();
			}else if(flag_carro2avia == 1){
				//flag_carro2avia = 0;
				gvd3_gam3b_tran();
			}
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status gam3a_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam3aEN;");
			counter_geral = 0;
			timeout = AMARELO_TIMEOUT;
			chVTSet(&vt_geral, S2ST(1), contador_geral_cb, aux);
			set_semaforos(5);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam3aEX;");
			piscar = 0;
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			gam3a_gvd1a_tran();
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

cb_status gam3b_cb(event_t ev){
	switch(ev){
		case ENTRY_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam3bEN;");
			counter_geral = 0;
			timeout = AMARELO_TIMEOUT;
			chVTSet(&vt_geral, S2ST(1), contador_geral_cb, aux);
			set_semaforos(5);
			return EVENT_HANDLED;
		case EXIT_EVENT:
			chprintf((BaseSequentialStream *)&SD1, "gam3bEX;");
			flag_carro2avia = 0;
			piscar = 0;
			return EVENT_HANDLED;
		case EVENT_TIMEOUT:
			gam3b_gvd2_tran();
			return EVENT_HANDLED;
	}
	return EVENT_NOT_HANDLED;
}

//Thread para leitura da serial, se o usuário mandar o valor "1"-> será setado EVENTO_CARRO_2VIA, "3" ->  Ambulância Começou a passar na principal.

static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg)
{
	uint16_t rc = 0;
    while (true) {
    	rc = sdGet(&SD1);
    	if (rc==0x31){
    		set_event(EVENT_CARRO_2VIA);
    	}
    	if (rc==0x32){
    		set_event(EVENT_PEDESTRE);
    	}
    	if (rc==0x33){
    		flag_ambu_principal = 1;
    		//set_event(EVENT_AMBULANCIA_PRINCIPAL);
    	}
    	if (rc==0x34){
    		set_event(EVENT_AMBULANCIA_PRINCIPAL_PASSOU);
    	}
    	if (rc==0x35){
    		flag_ambu_2avia = 1;
    		//set_event(EVENT_AMBULANCIA_2VIA);
    	}
    	if (rc==0x36){
    		//sdPut(&SD1, rc);
    		set_event(EVENT_AMBULANCIA_2VIA_PASSOU);
    	}

        chThdSleepMilliseconds(100);
    }
}

//Piscar LED, verificação se as Threads estão sendo executadas

static THD_WORKING_AREA(waThread2, 16);
static THD_FUNCTION(Thread2, arg)
{
    while (true) {
        palTogglePad(IOPORT2, PORTB_LED1);
        chThdSleepMilliseconds(1000);
    }
}

//Apenas para piscar o sinal vermelho de pedestres (equivalendo a um amarelo para pedestres)

static THD_WORKING_AREA(waThread3, 16);
static THD_FUNCTION(Thread3, arg)
{
    while (true) {
    	if (piscar==1){
        palTogglePad(IOPORT3, 4);
    	}
        chThdSleepMilliseconds(PERIODO_PISCAR_MS);
    }
}

//Thread que verifica se o sinal ficou em vermelho por pelo menos certo tempo antes de abrir para a ambuçância passar. (Definido no slide)

static THD_WORKING_AREA(waThread4, 16);
static THD_FUNCTION(Thread4, arg)
{
    while (true) {

    	if (flag_ambu_principal && counter_geral>=TEMPO_MINIMO_VERMELHO_AMBULANCIA){
    		flag_ambu_principal = 0;
    		set_event(EVENT_AMBULANCIA_PRINCIPAL);
    	}

    	if (flag_ambu_2avia && counter_geral>=TEMPO_MINIMO_VERMELHO_AMBULANCIA){
    		flag_ambu_2avia = 0;
    		set_event(EVENT_AMBULANCIA_2VIA);
    	}
        chThdSleepMilliseconds(100);
    }
}

static THD_WORKING_AREA(waThread5, 32);
static THD_FUNCTION(Thread5, arg)
{
    while (true) {
    	if (palReadPad(IOPORT4, 7)==PAL_LOW){
    		set_event(EVENT_CARRO_2VIA);
    	}
    	if (palReadPad(IOPORT4, 6)==PAL_LOW){
    		set_event(EVENT_PEDESTRE);
    	}
    	if (palReadPad(IOPORT3, 0)==PAL_LOW){
    		flag_ambu_principal = 1;
    	}
    	if (palReadPad(IOPORT3, 1)==PAL_LOW){
    		set_event(EVENT_AMBULANCIA_PRINCIPAL_PASSOU);
    	}
    	if (palReadPad(IOPORT3, 2)==PAL_LOW){
    		flag_ambu_2avia = 1;
    	}
    	if (palReadPad(IOPORT3, 3)==PAL_LOW){
    		set_event(EVENT_AMBULANCIA_2VIA_PASSOU);
    	}

        //palTogglePad(IOPORT4, 7);

        //PORTB ^= (1<<PB5);
        //PORTB ^= (1<<PORTB_LED1);

        chThdSleepMilliseconds(100);
    }
}


int main(void)
{

        halInit();
        chSysInit();

        sdStart(&SD1, 0);

        //input agora é na serial!
        //palSetPadMode(IOPORT4, 7, PAL_MODE_OUTPUT_PUSHPULL);
        palSetPadMode(IOPORT4, 7, PAL_MODE_INPUT_PULLUP); //input evento carro 2a via    PD7
        palSetPadMode(IOPORT4, 6, PAL_MODE_INPUT_PULLUP); //input evento pedestre        PD6
        palSetPadMode(IOPORT3, 0, PAL_MODE_INPUT_PULLUP); //input evento ambulância principal começou a passar   PC0
        palSetPadMode(IOPORT3, 1, PAL_MODE_INPUT_PULLUP); //input evento embulância principal passou             PC1
     	palSetPadMode(IOPORT3, 2, PAL_MODE_INPUT_PULLUP); //input evento ambulância 2a via começou a passar      PC2
        palSetPadMode(IOPORT3, 3, PAL_MODE_INPUT_PULLUP); //input evento ambulância 2a via passou                PC3


        //Configuração dos modos dos pinos usando o sintaxe do ChibiOS

        palSetPadMode(IOPORT2, 1, PAL_MODE_OUTPUT_PUSHPULL); //semaforo principal
        palSetPadMode(IOPORT2, 2, PAL_MODE_OUTPUT_PUSHPULL);
        palSetPadMode(IOPORT2, 3, PAL_MODE_OUTPUT_PUSHPULL);

        palSetPadMode(IOPORT3, 4, PAL_MODE_OUTPUT_PUSHPULL); //semaforo pedestre
        palSetPadMode(IOPORT3, 5, PAL_MODE_OUTPUT_PUSHPULL);

        palSetPadMode(IOPORT4, 2, PAL_MODE_OUTPUT_PUSHPULL); //semaforo 2a via
        palSetPadMode(IOPORT4, 3, PAL_MODE_OUTPUT_PUSHPULL);
        palSetPadMode(IOPORT4, 4, PAL_MODE_OUTPUT_PUSHPULL);

        palClearPad(LED_PORT, LED_PAD);

        //Criação das Threads

        chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);
        chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO+2, Thread2, NULL);
        chThdCreateStatic(waThread3, sizeof(waThread3), NORMALPRIO+3, Thread3, NULL);
        chThdCreateStatic(waThread4, sizeof(waThread4), NORMALPRIO+4, Thread4, NULL);
        chThdCreateStatic(waThread5, sizeof(waThread5), NORMALPRIO+5, Thread5, NULL);

        chVTSet(&vt_aux, S2ST(1), contador_aux_cb, 0);

        init_machine(init_cb);
       
        event_t ev;

        while(1){

            ev = wait_for_events();
            dispatch_event(ev);
        	//chThdSleepMilliseconds(100);
        }

        set_event(EMPTY_EVENT);
        
        return 0;
}
