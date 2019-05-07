#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>

//#include <dht.h>

#include <util/delay.h>

#define LED_PAD PORTB_LED1
#define LED_PORT IOPORT2

virtual_timer_t vt1, vt2;
int counter;

// dht DHT;
static uint8_t dht_buffer[5]; // DHT buffer


// void empirico_delay_us(uint16_t us)
// {
//     uint16_t i, j;
//     for(i=0; i<us; i++)
//         for(j=0; j<261; j++)
//             _NOP();
// }


void expiration_cb(void *par)
{
    palTogglePad(LED_PORT, LED_PAD);
    chVTSet(&vt1, MS2ST(150), expiration_cb, 0);
}

int anterior = 0, atual = 0;

void serial_cb(void *par)
{
    //chprintf((BaseSequentialStream *)&SD1, "%d\n", counter++);
    // DHT.read11(7); //Lê as informações do sensor
    // chprintf((BaseSequentialStream *)&SD1, "RAFAEL Umidade = %d %, Temperadura = %d C\n", DHT.humidity, DHT.temperature);

	// if (read_DHT11(&dht_buffer[0]) == DHT11_OK)
	// 	{
	// 	  chprintf((BaseSequentialStream*)&SD1, "  humidity: %02d %%\n\r", dht_buffer[0]);
	// 	}


	palSetPadMode(IOPORT4, 7, PAL_MODE_OUTPUT_PUSHPULL);
    	palClearPad(IOPORT4, 7);
    	palSetPad(IOPORT2, 5);
    	_delay_ms(30);
    	//chThdSleepMilliseconds(18);
    	palSetPad(IOPORT4, 7);
    	palClearPad(IOPORT2, 5);
    	_delay_us(80);
    	palClearPad(IOPORT4, 7);
    	//chThdSleepMicroseconds(40);
    	palSetPadMode(IOPORT4, 7, PAL_MODE_INPUT_PULLUP);


    chVTSet(&vt2, S2ST(2), serial_cb, 0);

    //atual = ST2MS(chVTGetSystemTime());
    //chprintf((BaseSequentialStream *)&SD1, "%d\n", ST2MS(chVTTimeElapsedSinceX(MS2ST(anterior))));

    //chprintf((BaseSequentialStream *)&SD1, "%d\n", ST2US(chVTGetSystemTime()));
    //empirico_delay_us(1000);
    //_delay_us(1000);
    //chprintf((BaseSequentialStream *)&SD1, "%d\n", ST2US(chVTGetSystemTime()));
    //chprintf((BaseSequentialStream *)&SD1, "1seg delay\n");
    //anterior  = atual;



}

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD1, 0);

    palClearPad(LED_PORT, LED_PAD);


    // digitalWrite(13, HIGH);

    //palSetPadMode(IOPORT4, 7, PAL_MODE_INPUT_PULLUP);

    //chVTSet(&vt1, MS2ST(150), expiration_cb, 0);
    chVTSet(&vt2, S2ST(2), serial_cb, 0);


    //chprintf((BaseSequentialStream *)&SD1, "%d\n", ST2MS(chVTGetSystemTime()));
    //chprintf((BaseSequentialStream *)&SD1, "%d\n", ST2MS(chVTGetSystemTime()));
    //chprintf((BaseSequentialStream *)&SD1, "%d\n", ST2MS(chVTGetSystemTime()));
   
     //    palSetPadMode(IOPORT4, 7, PAL_MODE_OUTPUT_PUSHPULL);
    	// palClearPad(IOPORT4, 7);
    	// chThdSleepMilliseconds(18);
    	// palSetPad(IOPORT4, 7);
    	// chThdSleepMicroseconds(40);
    	// palSetPadMode(IOPORT4, 7, PAL_MODE_INPUT_PULLUP);


     //   palSetPadMode(IOPORT4, 7, PAL_MODE_OUTPUT_PUSHPULL);
    	// palClearPad(IOPORT4, 7);
    	// palSetPad(IOPORT2, 5);
    	// _delay_ms(18);
    	// //chThdSleepMilliseconds(18);
    	// palSetPad(IOPORT4, 7);
    	// palClearPad(IOPORT2, 5);
    	// _delay_us(40);
    	// //chThdSleepMicroseconds(40);
    	// palSetPadMode(IOPORT4, 7, PAL_MODE_INPUT_PULLUP);

    while (1) {


    	// palSetPadMode(IOPORT4, 7, PAL_MODE_OUTPUT_PUSHPULL);
    	// palClearPad(IOPORT4, 7);
    	// palSetPad(IOPORT2, 5);

    	// chThdSleepMilliseconds(18);
    	// palSetPad(IOPORT4, 7);
    	// palClearPad(IOPORT2, 5);
    	// chThdSleepMicroseconds(40);
    	// palSetPadMode(IOPORT4, 7, PAL_MODE_INPUT_PULLUP);
    	//chThdSleepMicroseconds(40);
    	// palSetPadMode(IOPORT4, 7, PAL_MODE_OUTPUT_PUSHPULL);
    	// palClearPad(IOPORT4, 7);
    	// chThdSleepMilliseconds(18);
    	// palSetPadMode(IOPORT4, 7);
    	// chThdSleepMicroseconds(40);
    	// palSetPadMode(IOPORT4, 7, PAL_MODE_INPUT_PULLUP);

    
    	if (palReadPad(IOPORT4, 7)==PAL_HIGH){
    		sdPut(&SD1, 49);
    	}else if (palReadPad(IOPORT4, 7)==PAL_LOW){
    		sdPut(&SD1, 48);    	
    	}
    	chThdSleepMilliseconds(1);

    	//chprintf((BaseSequentialStream *)&SD1, "%d\n", ST2MS(chVTGetSystemTime()));
        //rv = DHT.read11(7); //Lê as informações do sensor
        //chprintf((BaseSequentialStream *)&SD1, "%d\n", ST2MS(chVTGetSystemTime()));
        //chprintf((BaseSequentialStream *)&SD1, "rv-> %d bits[0] -> %d bits[1] -> %d\n", rv, DHT.bits[0], DHT.bits[1]);
        //rv = ST2MS(chVTGetSystemTime());
        //chprintf((BaseSequentialStream *)&SD1, "timenow= %d\n", rv);
        //delay(1000);
       
        //chThdSleepMilliseconds(1500);

    }

    return 0;
}
