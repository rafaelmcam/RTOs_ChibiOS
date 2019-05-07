#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>


/* 
 * Constantes utilizadas no código 
 */
#define FOSC 16000000ul  /* velocidade do relógio */
#define BAUD 115200      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */

#define BUFFER_SIZE 20   /* Tamanho do buffer da USART */


/* 
 * Definição de funções 
 */

void setup_int0(void)
{
    /*
     * Para as macros abaixo e outras, veja o arquivo
     * usr/lib/avr/include/avr/iom328p.h
     */
    EICRA = (1<< ISC00);
    EIMSK |= (1 << INT0);
    EIFR  = (1 << INTF0);
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

/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= (1 << LED_PINNBR);
///////////////////////////////////////
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);
///////////////////////////////////////


    /* Start transmitting */
    UDR0 = 0;
    sei(); /* Seta flag principal de habilitação de interrupções */
}

/* -------------------------------------*/
void delay_ms(uint16_t ms)
{
    uint16_t i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<4000; j++)
            _NOP();
}

/* -------------------------------------*/
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

/* -------------------------------------*/
/*
 * Interrupt handlers devem ser definidos com a macros
 * ISR(), que é definida no arquivo avr/interrupt.h.
 * Veja também a página
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
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


ISR(INT0_vect)
{

	//disabilitando ou nao a transmição se conectar PD2 no terra e mesmo valor lógico no LED
	UCSR0B = (UCSR0B & (1<<TXEN0))?(UCSR0B & ~ (1<<TXEN0)):(UCSR0B|(1<<TXEN0));

 
    LED_PIN |= (1<<LED_PINNBR);
}

/* -------------------------------------*/
int main(void)
{
    char msg[] = "Hello, world\n";
    uint8_t i;


    setup_int0();
    setup_usart();
    setup_main();

    while(1) {
        for(i=0; i<sizeof(msg); i++)
            if(write(msg[i])){
            	PORTB |= (1<<LED_PINNBR);//caso buffer for pequeno demais acender LED
                break;
            }
            delay_ms(1500);
    }

    return 0;
}
