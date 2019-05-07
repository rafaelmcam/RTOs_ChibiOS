#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>



#define FOSC 16000000ul  /* velocidade do relógio */
#define BAUD 115200      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */

#define BUFFER_SIZE 20   /* Tamanho do buffer da USART */



void setup_timer1(void)
{
  

    //usar sempre o H na frente do L se for dividir o OCR1A em OCR1AH e OCR1AL

    //  OCR1A = 0xFFFF;

    //TCCR1A = 0; NÃO FAÇA ISSO! TEM BITS RESERVADOS, QUE VC NAO PODE DIZER QUE É 0

    TCCR1B = (1 << CS12) | (1<<CS10)|(1<<ICES1);
    TIMSK1 |= (1<<ICIE1);
    sei();
}


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


volatile uint8_t byte_high=0;
volatile uint8_t byte_low=0;
volatile uint16_t byte_geral;



volatile uint8_t flag = 0;

ISR(TIMER1_CAPT_vect){


    byte_geral = ICR1;



    byte_high = (byte_geral>>8);
    byte_low = byte_geral & 0xFF;
    

    flag = 1;
    LED_PIN |= (1<<LED_PINNBR);

    //desnecessário por causa da falta de debounce?
    TCCR1B ^= (1<<ICES1);
}


/* -------------------------------------*/
int main(void)
{   

    uint16_t ms = 2000;
   

    setup_usart();
    setup_timer1();
    setup_main();


    uint8_t i;
    

    
    while(1) {
        
        
        //primeiro mandar o low byte
        write(byte_low);
        write(byte_high);

        //write(0xFFFF);

        delay_ms(150);

    }

    return 0;
}
