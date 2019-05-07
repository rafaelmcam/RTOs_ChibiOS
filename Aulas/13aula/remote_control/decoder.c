#include "sm_decoder.h"
#include "event.h"
#include "usart.h"
#include "timer.h"
#include "debug.h"


int main(void) {
	event_t ev;

	init_machine();
	DEBUG_WRITE_START("Start!");

	while(1) {
		ev = wait_for_events();
		dispatch_event(ev);
	}

	return 0;
}
