#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"


#define FOSC 16000000ul  /* velocidade do relógio */
#define BAUD 115200      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8ul*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */

#define BUFFER_SIZE 20   /* Tamanho do buffer da USART */

/* ---------------------------------------------- */
void setup_usart(void)
{
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

/* ---------------------------------------------- */
uint8_t _tx_buffer[BUFFER_SIZE];     /* buffer para transmissão */
uint8_t _tx_head, _tx_tail;   /* ponteiros para o buffer circular */
uint8_t _usart_transmitting=0;

uint8_t write(uint8_t c)
{
	uint8_t rc = 0;

	cli();
	if (!_usart_transmitting) {
		UDR0 = c;
		UCSR0B |= (1 << UDRIE0);
		_usart_transmitting = 1;
	} else {
		uint8_t next = _tx_head + 1;
		if (next >= BUFFER_SIZE)
			next = 0;
		if (next != _tx_tail) { /* buffer cheio */
			_tx_buffer[_tx_head] = c;
			_tx_head = next;
		} else
			rc = 1;
	}
	sei();
	return rc;
}


/* ---------------------------------------------- */
uint8_t writeln(char s[])
{
	char* p = s;
	uint8_t rc = 0;

	while (*p && !rc)
		rc = write(*p++);

	return rc;
}

/* ---------------------------------------------- */
ISR(USART_UDRE_vect)
{
	if (_tx_head == _tx_tail) {
		UCSR0B &= ~(1 << UDRIE0);
		_usart_transmitting = 0;
	} else {
		UDR0 = _tx_buffer[_tx_tail];
		if (++_tx_tail >= BUFFER_SIZE)
			_tx_tail = 0;
	}
}
