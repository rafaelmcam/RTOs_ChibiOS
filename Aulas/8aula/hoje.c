#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>



#define FOSC 16000000ul  /* velocidade do relógio */
#define BAUD 115200      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */

#define BUFFER_SIZE 20   /* Tamanho do buffer da USART */





void setup_adc(void){
    
    
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADEN)  | (1<<ADSC) | (1<<ADIE) | (1<<ADATE);


  //  ADCSRB |= (1<<ADTS0);
    
    ADMUX |=  (1<<REFS0);

  //  ADMUX &= 0xf0;
    
    DIDR0 |= (1<<ADC0D);
    
}

void setup_timer0(void)
{
  

    //usar sempre o H na frente do L se for dividir o OCR1A em OCR1AH e OCR1AL

  //  OCR1A = 0xFFFF;

    //TCCR1A = 0; NUNCA FAÇA ISSO! TEM BITS RESERVADOS, QUE VC NAO PODE DIZER QUE É 0

    TCCR0B = (1 << CS02) | (1<<CS00);
    TIMSK0 |= (1<<TOIE0);
    sei();
}


/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= (1 << LED_PINNBR);
  //  LED_PORT |= (1 << LED_PINNBR);

    DDRB  &= ~(1<<PB0);
    PORTB |= (1<<PB0);
    
    
    DDRC &= ~(1<<PC0);
   // PORTC &= ~(1<<PC0);

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


// volatile uint16_t contar=0;

// ISR(TIMER1_COMPA_vect)
// {
//     contar++;
// }


uint8_t * toArray(uint8_t number, uint8_t* numberArray)
    {
        
        int i;
    

        for ( i = 7; i >=0; i--)
        {
            numberArray[7-i] = (number&(1u<<i))?'1':'0';
        }
        return numberArray;
    }


volatile uint8_t capturado=0;
volatile uint8_t capturado2=0;

volatile char msg[8];
volatile char msg2[8];

volatile uint8_t flag = 0;

ISR(TIMER0_OVF_vect){
    
    flag++;
    
    
}



uint8_t flag_adc = 0 ;


ISR(ADC_vect){
    
    
    flag_adc = 1;
   // LED_PORT |= (1<<LED_PINNBR);
    
    
}


/* -------------------------------------*/


uint8_t bit1, bit2;
uint16_t bit_geral;


int main(void)
{   

    uint16_t ms = 2000;
   
    
    setup_adc();
    setup_usart();
    setup_timer0();
    setup_main();


    uint8_t i;
    
    
    while(1) {
        
        if (flag>=30){
            

              //  ADCSRA |= ( 1 << ADSC );

                if (flag_adc){

                bit_geral=ADC;

                bit1 = (bit_geral>>8);
                bit2 = bit_geral & 0xFF;

                write(bit2);
                write(bit1);
                
            //     toArray(bit1, msg);
            //     toArray(bit2, msg2);
                
                
            //     for(i=0; i<sizeof(msg); i++)
            //         if(write(msg[i])){
            //             PORTB |= (1<<LED_PINNBR);//caso buffer for pequeno demais acender LED
            //             break;
            // }

            //     for(i=0; i<sizeof(msg2); i++)
            //         if(write(msg2[i])){
            //             PORTB |= (1<<LED_PINNBR);//caso buffer for pequeno demais acender LED
            //             break;
            //     }

            // write('\n');

                 //   ADCSRA |= ( 1 << ADIF );


              flag_adc=0;  
             }

            
          //  LED_PIN |= (1<<LED_PINNBR);
            flag=0;
        }
        
    }

    return 0;
}
