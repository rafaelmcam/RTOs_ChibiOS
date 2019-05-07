#ifndef BSP_H
#define BSP_H

#ifdef __AVR__
#include "bsp_avr.h"
#elif defined __linux__
#include "bsp_linux.h"
#else
#error "Architecture not know!"
#endif

#endif /* BSP_H */
