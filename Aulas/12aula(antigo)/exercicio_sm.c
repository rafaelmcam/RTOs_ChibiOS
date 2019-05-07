#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define FOSC 16000000ul  /* velocidade do relógio */
#define FTIM FOSC/8/1000

#define BAUD 115200      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */

#define BUFFER_SIZE 20   /* Tamanho do buffer da USART */
#define MAX_EVENTS 16

#define MS1L  3.0*FTIM
#define MS1H 14.0*FTIM
#define MS2L  2.5*FTIM
#define MS2H  6.0*FTIM
#define MS3L  0.4*FTIM
#define MS3H  2.0*FTIM
#define MS4L  1.05*FTIM
#define MS4H  2.0*FTIM
#define MS5L  0.2*FTIM
#define MS5H  1.0*FTIM
#define MS6L 12.0*FTIM
#define MS6H 28.0*FTIM


typedef uint16_t event_t;
volatile event_t events;

enum {
    EVENT0  = 0X0001,
    EVENT1  = 0X0002,
    EVENT2  = 0X0004,
    EVENT3  = 0X0008,
    EVENT4  = 0X0010,
    EVENT5  = 0X0020,
    EVENT6  = 0X0040,
    EVENT7  = 0X0080,
    EVENT8  = 0X0100,
    EVENT9  = 0X0200,
    EVENT10 = 0X0400,
    EVENT11 = 0X0800,
    EVENT12 = 0X1000,
    EVENT13 = 0X2000,
    EVENT14 = 0X4000,
    EVENT15 = 0X8000
};



/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= (1 << LED_PINNBR);
  //  LED_PORT |= (1 << LED_PINNBR);

    DDRB  &= ~(1<<PB0);
    PORTB |= (1<<PB0);

    sei(); 
}

void setup_usart(void)
{
    /*
     * Para as macros abaixo e outras, veja o arquivo
     * /usr/lib/avr/include/avr/iom328p.h
     */
    /*
     * Primeiro setamos a direção certa para os pinos RX e TX
     */
    DDRD &= ~(1 << PD0);  /* USART RX bit para a placa Nano */
    DDRD |= (1 << PD1);  /* USART TX bit para a placa Nano */
    
    /*
     * Agora configuramos a USART
     */
    UBRR0H = (MYUBRR >> 8) & 0xF;
    UBRR0L = MYUBRR & 0xFF;
    UCSR0A = (1 << U2X0); /* double speed */
    UCSR0B = (1 << TXEN0); /* habilita a transmissão mas não a interrupção (não ainda) */
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);  /* Asynchronous, 8-N-1 */
}

void delay_ms(uint16_t ms)
{
    uint16_t i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<4000; j++)
            _NOP();
}


uint16_t tx_buffer[BUFFER_SIZE];     /* buffer para transmissão */
uint8_t tx_head, tx_tail;   /* ponteiros para o buffer circular */
uint8_t usart_transmitting=0;

uint8_t write(uint8_t c)
{
    uint8_t rc = 0;

    cli();
    // primeiro if inicialmente sempre executado pois usart_transmitting começa 0, habilita a interrupção de UDR vazio
    if (!usart_transmitting) {
        UDR0 = c;
        UCSR0B |= (1 << UDRIE0);
        usart_transmitting = 1;
    } else {
        uint8_t next = tx_head + 1;
        if (next >= BUFFER_SIZE)
            next = 0;
        if (next != tx_tail) { /* buffer cheio */
            tx_buffer[tx_head] = c;
            tx_head = next;
        } else
            rc = 1;
    }
    sei();
    return rc;
}


void write_string(uint8_t string[]){
    
    int i;
    for (i=0; string[i]!='\0'; i++){
        write(string[i]);
        
    }
    
}

ISR(USART_UDRE_vect)
{
    if (tx_head == tx_tail) {
        UCSR0B &= ~(1 << UDRIE0);
        usart_transmitting = 0;
    } else {
        UDR0 = tx_buffer[tx_tail];
        if (++tx_tail >= BUFFER_SIZE)
            tx_tail = 0;
    }
}


event_t wait_for_events(void)
{
    uint8_t ev;
    event_t copy;

    while(!events)
        ;

    for(ev=0, copy=events; ev<MAX_EVENTS && copy; ev++, copy>>=1)
        if (copy & 0x1) {
            cli();
            events &= ~_BV(ev);
            sei();
            break;
        }

    return _BV(ev);
}

volatile uint16_t t2, t1, delta;
volatile uint32_t big;
volatile uint8_t tmp;

volatile uint8_t flag = 0;
volatile uint8_t l_edge = 0;


void setup_timer1(void)
{
  
    TCCR1B = (1 << CS11) | (1<<WGM12);

    OCR1A = 0xFFFF;

    TIMSK1 |= (1<<ICIE1) | (1<<OCIE1A);
    sei();
}


ISR(TIMER1_CAPT_vect){

    t2 = ICR1;
    flag = 1;

    if (!l_edge){
    	l_edge = 1;
    	t1 = t2;
    	TCCR1B ^= _BV(ICES1);
    	return;
    }

    if (t2 < t1) {
		big = (1l << 16) + t2 - t1;
		delta = big;
	}
	else
		delta = t2 - t1;
	t1 = t2;

	tmp = TCCR1B;
      
	if (tmp & _BV(ICES1)) { /* Borda de subida, intervalo off */
		TCCR1B = tmp & ~_BV(ICES1);

		if ((delta < MS1H) && (delta > MS1L))
			events |= EVENT1;
		else if ((delta < MS3H) && (delta > MS3L))
			events |= EVENT3;
	} else { /* Borda de descida, intervalo on */
		TCCR1B = tmp | _BV(ICES1);

		if ((delta < MS2H) && (delta > MS2L))
			events |= EVENT2;
		else if ((delta < MS4H) && (delta > MS4L))
			events |= EVENT4;
		else if ((delta < MS5H) && (delta > MS5L))
			events |= EVENT5;
		else if ((delta < MS6H) && (delta > MS6L))
			events |= EVENT6;
	}  
}


ISR(TIMER1_COMPA_vect)
{
	if (!flag) {
		TCCR1B &= ~ (1<<ICES1);
		l_edge = 0;
		events |= EVENT7;
	}
	flag = 0;
}

typedef enum{
    estado1 = 0, estado2, estado3, estado4
}estados;

typedef enum{
    saida1 = 0, saida2, saida3, saida4
}saidas;


uint8_t _bytes[5];
uint8_t _idx_byte, _idx_bit;

void push_bit(uint8_t bit){
    
    if (bit)
        _bytes[_idx_byte] = _BV(_idx_bit);
    if(++_idx_bit >= 8){
        _idx_byte++;
        _idx_bit = 0;
    }
}

void send_bytes(void){
    
    uint8_t i;
    
    for(i=0; i<sizeof(_bytes); i++){
        write(_bytes[i]);
        _bytes[i] = 0;  
    }
    _idx_byte = _idx_bit = 0;
}

int main(){
       
    setup_usart();
    setup_timer1();
    
    estados state = estado1;
    
    //inputs input = input1, input2, input3, input4, input5, input6, input7;
    saidas saida;

    event_t ev;
    
    
    write_string("comeco\n");
    
    //write('c');
    
   while(1){
            
            ev = wait_for_events();
            
            switch(state){
                
                case estado1:
                    switch (ev){
                        case EVENT1:{
                            state = estado2;
                            
                            write_string("11\n");
                            
                            break;
                    }
                        default:
                            {
                                state = estado1;
                                break;
                            }
                    }       
                    break;
                
                
                    
                case estado2:
                    switch (ev){

                        case EVENT2:{
                            state = estado3;
                            write_string("22\n");
                            break;
                    }
                        default:
                            {
                                state = estado1;
                                write_string("21\n");
                                break;
                            }
                    }       
                    break;
                
                
                
                case estado3:
                    switch (ev){
                        case EVENT3:{
                            state = estado4;
                            write_string("33\n");
                            break;
                    }
                        default:
                            {
                                state = estado1;
                                break;
                            }
                    }       
                    break;
                
                
                    
                case estado4:
                    switch (ev){
                        case EVENT4:{
                            state = estado3;
                            //saida = saida1;
                            write_string("44\n");
                            
                            push_bit(1);
                            
                            break;
                    }
                        case EVENT5:{
                            state = estado3;
                            
                            push_bit(0);
                            
                            write_string("45\n");
                            
                            //saida = saida2;
                            break;
                        }
                        case EVENT6:{
                            state = estado3;
                            
                            write_string("46\n");
                            
                            //saida = saida3;
                            break;
                        }
                        case EVENT7:{
                            state = estado1;
                            
                            write_string("47\n");
                          // saida = saida4;
                            
                          //  send_bytes();
                            break;
                        }
                        
                        default:
                            {
                            state = estado1;
                            break;
                            }
                    }       
                    break;
                         
                
            }
              
            //write(state);
    
    //chave do while
   }
    
    return 0;
}
