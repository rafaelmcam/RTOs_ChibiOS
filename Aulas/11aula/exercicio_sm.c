#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <stdio.h>





#define FOSC 16000000ul  /* velocidade do relógio */
#define BAUD 115200      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */

#define BUFFER_SIZE 20   /* Tamanho do buffer da USART */



#define MAX_EVENTS 16


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






//precisa mesmo da usart????





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
    
    return ev;
}







volatile uint8_t byte_high=0;
volatile uint8_t byte_low=0;
volatile uint16_t byte_geral, t2, t1, delta;
volatile uint32_t temp;



volatile uint8_t flag = 0;




void setup_timer1(void)
{
  

	//  OCR1A = 0xFFFF;

    //TCCR1A = 0; NÃO FAZER ISSO! TEM BITS RESERVADOS, QUE VC NAO PODE DIZER QUE É 0

   
    TCCR1B = (1 << CS11) | (1<<ICES1);
    
    
    //testar com cancelador de noise
   // TCCR1B = (1<<ICNC1) | (1 << CS11) | (1<<ICES1);
    
    
    
    TIMSK1 |= (1<<ICIE1);
    sei();
}





ISR(TIMER1_CAPT_vect){

    TCCR1B ^= (1<<ICES1);
    byte_geral = ICR1;



    byte_high = (byte_geral>>8);
    byte_low = byte_geral & 0xFF;
    

    
   // LED_PIN |= (1<<LED_PINNBR);

    
    
    if(!flag){
        t1 = (byte_high<<8)|byte_low;
        flag = 1;
        return;
    }
    
    
    t2 = (byte_high<<8)|byte_low;
    if (t2<t1)
        delta = (1<<16)+t2-t1;
    
    if (t1<t2)
        delta = (uint32_t)(1<<16)+t2-t1;
    else
        delta = t2 - t1;
    
    if ((delta>14400)&&(delta<21600)&&!(TCCR1B&&(1<<ICES1)))
        events |= EVENT1;
    else if ((delta>7200)&&(delta<10800)&&(TCCR1B&&(1<<ICES1)))
        events |= EVENT2;
    else if ((delta>1760)&&(delta<2640)&&!(TCCR1B&&(1<<ICES1)))
        events |= EVENT3;
    else if ((delta>1760)&&(delta<2640)&&(TCCR1B&&(1<<ICES1)))
        events |= EVENT4;
    else if ((delta>800)&&(delta<1200)&&(TCCR1B&&(1<<ICES1)))
        events |= EVENT5;
    else if ((delta>40000)&&(delta<60000)&&(TCCR1B&&(1<<ICES1)))
        events |= EVENT6;
    else if ((delta>62000)&&(TCCR1B&&(1<<ICES1)))
        events |= EVENT7;
    
    
    
    t1 = t2;
    flag = 0;
}







typedef enum{
    
    estado1 = 0, estado2, estado3, estado4

}estados;

typedef enum{
    saida1 = 0, saida2, saida3, saida4
}saidas;




int main(){

    estados state = estado1;
    
    //inputs input = input1, input2, input3, input4, input5, input6, input7;
    saidas saida;
    
   while(1){
            
            events = wait_for_events();
            
            switch(state){
                
                case estado1:
                    switch (events){
                        case EVENT1:{
                            state = estado2;
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
                    switch (events){
                        case EVENT2:{
                            state = estado3;
                            break;
                    }
                        default:
                            {
                                state = estado1;
                                break;
                            }
                    }       
                    break;
                
                
                
                case estado3:
                    switch (events){
                        case EVENT3:{
                            state = estado4;
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
                    switch (events){
                        case EVENT4:{
                            state = estado3;
                            saida = saida1;
                            break;
                    }
                        case EVENT5:{
                            state = estado3;
                            saida = saida2;
                            break;
                        }
                        case EVENT6:{
                            state = estado3;
                            saida = saida3;
                            break;
                        }
                        case EVENT7:{
                            state = estado1;
                            saida = saida4;
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
            
            
            write(state);
    
    //chave do while
   }
    
    
    
    
    
    return 0;
}
