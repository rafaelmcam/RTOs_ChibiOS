#ifndef EVENT_H
#define EVENT_H

#include <avr/io.h>

typedef uint16_t event_t;

extern event_t events;

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

#endif /* EVENT_H*/
