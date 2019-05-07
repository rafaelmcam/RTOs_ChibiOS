#include "IRremote.h"
#include "IRremoteInt.h"

// class decode_results
// {
// 	public:
// 		decode_type_t          decode_type;  // UNKNOWN, NEC, SONY, RC5, ...
// 		unsigned int           address;      // Used by Panasonic & Sharp [16-bits]
// 		unsigned long          value;        // Decoded value [max 32-bits]
// 		int                    bits;         // Number of bits in decoded value
// 		volatile unsigned int  *rawbuf;      // Raw intervals in 50uS ticks
// 		int                    rawlen;       // Number of records in rawbuf
// 		int                    overflow;     // true iff IR raw code too long
// };

// class IRrecv
// {
// 	public:
// 		IRrecv (int recvpin) ;
// 		IRrecv (int recvpin, int blinkpin);

// 		void  blink13    (int blinkflag) ;
// 		int   decode     (decode_results *results) ;
// 		void  enableIRIn ( ) ;
// 		bool  isIdle     ( ) ;
// 		void  resume     ( ) ;

// 	private:
// 		long  decodeHash (decode_results *results) ;
// 		int   compare    (unsigned int oldval, unsigned int newval) ;

// 		//......................................................................
// #		if (DECODE_RC5 || DECODE_RC6)
// 			// This helper function is shared by RC5 and RC6
// 			int  getRClevel (decode_results *results,  int *offset,  int *used,  int t1) ;
// #		endif
// #		if DECODE_RC5
// 			bool  decodeRC5        (decode_results *results) ;
// #		endif
// #		if DECODE_RC6
// 			bool  decodeRC6        (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_NEC
// 			bool  decodeNEC        (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_SONY
// 			bool  decodeSony       (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_PANASONIC
// 			bool  decodePanasonic  (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_JVC
// 			bool  decodeJVC        (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_SAMSUNG
// 			bool  decodeSAMSUNG    (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_WHYNTER
// 			bool  decodeWhynter    (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_AIWA_RC_T501
// 			bool  decodeAiwaRCT501 (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_LG
// 			bool  decodeLG         (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_SANYO
// 			bool  decodeSanyo      (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_MITSUBISHI
// 			bool  decodeMitsubishi (decode_results *results) ;
// #		endif
// 		//......................................................................
// #		if DECODE_DISH
// 			bool  decodeDish (decode_results *results) ; // NOT WRITTEN
// #		endif
// 		//......................................................................
// #		if DECODE_SHARP
// 			bool  decodeSharp (decode_results *results) ; // NOT WRITTEN
// #		endif
// 		//......................................................................
// #		if DECODE_DENON
// 			bool  decodeDenon (decode_results *results) ;
// #		endif
// //......................................................................
// #		if DECODE_LEGO_PF
// 			bool  decodeLegoPowerFunctions (decode_results *results) ;
// #		endif
// } ;

// //------------------------------------------------------------------------------
// // Main class for sending IR
// //
// class IRsend
// {
// 	public:
// 		IRsend () { }

// 		void  custom_delay_usec (unsigned long uSecs);
// 		void  enableIROut 		(int khz) ;
// 		void  mark        		(unsigned int usec) ;
// 		void  space       		(unsigned int usec) ;
// 		void  sendRaw     		(const unsigned int buf[],  unsigned int len,  unsigned int hz) ;

// 		//......................................................................
// #		if SEND_RC5
// 			void  sendRC5        (unsigned long data,  int nbits) ;
// #		endif
// #		if SEND_RC6
// 			void  sendRC6        (unsigned long data,  int nbits) ;
// #		endif
// 		//......................................................................
// #		if SEND_NEC
// 			void  sendNEC        (unsigned long data,  int nbits) ;
// #		endif
// 		//......................................................................
// #		if SEND_SONY
// 			void  sendSony       (unsigned long data,  int nbits) ;
// #		endif
// 		//......................................................................
// #		if SEND_PANASONIC
// 			void  sendPanasonic  (unsigned int address,  unsigned long data) ;
// #		endif
// 		//......................................................................
// #		if SEND_JVC
// 			// JVC does NOT repeat by sending a separate code (like NEC does).
// 			// The JVC protocol repeats by skipping the header.
// 			// To send a JVC repeat signal, send the original code value
// 			//   and set 'repeat' to true
// 			void  sendJVC        (unsigned long data,  int nbits,  bool repeat) ;
// #		endif
// 		//......................................................................
// #		if SEND_SAMSUNG
// 			void  sendSAMSUNG    (unsigned long data,  int nbits) ;
// #		endif
// 		//......................................................................
// #		if SEND_WHYNTER
// 			void  sendWhynter    (unsigned long data,  int nbits) ;
// #		endif
// 		//......................................................................
// #		if SEND_AIWA_RC_T501
// 			void  sendAiwaRCT501 (int code) ;
// #		endif
// 		//......................................................................
// #		if SEND_LG
// 			void  sendLG         (unsigned long data,  int nbits) ;
// #		endif
// 		//......................................................................
// #		if SEND_SANYO
// 			void  sendSanyo      ( ) ; // NOT WRITTEN
// #		endif
// 		//......................................................................
// #		if SEND_MISUBISHI
// 			void  sendMitsubishi ( ) ; // NOT WRITTEN
// #		endif
// 		//......................................................................
// #		if SEND_DISH
// 			void  sendDISH       (unsigned long data,  int nbits) ;
// #		endif
// 		//......................................................................
// #		if SEND_SHARP
// 			void  sendSharpRaw   (unsigned long data,  int nbits) ;
// 			void  sendSharp      (unsigned int address,  unsigned int command) ;
// #		endif
// 		//......................................................................
// #		if SEND_DENON
// 			void  sendDenon      (unsigned long data,  int nbits) ;
// #		endif
// 		//......................................................................
// #		if SEND_PRONTO
// 			void  sendPronto     (char* code,  bool repeat,  bool fallback) ;
// #		endif
// //......................................................................
// #		if SEND_LEGO_PF
// 			void  sendLegoPowerFunctions (uint16_t data, bool repeat = true) ;
// #		endif
// } ;

// #endif


//+=============================================================================
void  IRsend::sendRaw (const unsigned int buf[],  unsigned int len,  unsigned int hz)
{
	// Set IR carrier frequency
	enableIROut(hz);

	for (unsigned int i = 0;  i < len;  i++) {
		if (i & 1)  space(buf[i]) ;
		else        mark (buf[i]) ;
	}

	space(0);  // Always end with the LED off
}

//+=============================================================================
// Sends an IR mark for the specified number of microseconds.
// The mark output is modulated at the PWM frequency.
//
void  IRsend::mark (unsigned int time)
{
	TIMER_ENABLE_PWM; // Enable pin 3 PWM output
	if (time > 0) custom_delay_usec(time);
}

//+=============================================================================
// Leave pin off for time (given in microseconds)
// Sends an IR space for the specified number of microseconds.
// A space is no output, so the PWM output is disabled.
//
void  IRsend::space (unsigned int time)
{
	TIMER_DISABLE_PWM; // Disable pin 3 PWM output
	if (time > 0) IRsend::custom_delay_usec(time);
}





//+=============================================================================
// Enables IR output.  The khz value controls the modulation frequency in kilohertz.
// The IR output will be on pin 3 (OC2B).
// This routine is designed for 36-40KHz; if you use it for other values, it's up to you
// to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
// TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
// controlling the duty cycle.
// There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
// To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
// A few hours staring at the ATmega documentation and this will all make sense.
// See my Secrets of Arduino PWM at http://arcfn.com/2009/07/secrets-of-arduino-pwm.html for details.
//
void  IRsend::enableIROut (int khz)
{
// FIXME: implement ESP32 support, see IR_TIMER_USE_ESP32 in boarddefs.h
#ifndef ESP32
	// Disable the Timer2 Interrupt (which is used for receiving IR)
	TIMER_DISABLE_INTR; //Timer2 Overflow Interrupt

	pinMode(TIMER_PWM_PIN, OUTPUT);
	digitalWrite(TIMER_PWM_PIN, LOW); // When not sending PWM, we want it low

	// COM2A = 00: disconnect OC2A
	// COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
	// WGM2 = 101: phase-correct PWM with OCRA as top
	// CS2  = 000: no prescaling
	// The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
	TIMER_CONFIG_KHZ(khz);
#endif
}

//+=============================================================================
// Custom delay function that circumvents Arduino's delayMicroseconds limit

void IRsend::custom_delay_usec(unsigned long uSecs) {
  if (uSecs > 4) {
    unsigned long start = micros();
    unsigned long endMicros = start + uSecs - 4;
    if (endMicros < start) { // Check if overflow
      while ( micros() > start ) {} // wait until overflow
    }
    while ( micros() < endMicros ) {} // normal wait
  } 
  //else {
  //  __asm__("nop\n\t"); // must have or compiler optimizes out
  //}
}

