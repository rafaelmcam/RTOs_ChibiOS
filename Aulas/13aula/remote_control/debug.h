#ifndef DEBUG_H
#define DEBUG_H

#include "usart.h"

#ifdef DEBUG
#define DEBUG_WRITE_STATE(st) do {		\
		writeln(st); write(' ');        \
	} while (0)

#define DEBUG_WRITE_STATE_EVENT(st, ev) do {    \
		writeln(st); write(' ');	\
		write_word(ev); write('\n');	\
	} while (0)
#define DEBUG_WRITE_START(msg) do {		\
		writeln(msg); write('\n');	\
	} while(0)
#else /* !DEBUG */
#define DEBUG_WRITE_START(msg)
#define DEBUG_WRITE_STATE_EVENT(st, ev)
#define DEBUG_WRITE_START(msg)
#endif /* DEBUG */

#endif /* DEBUG_H */
