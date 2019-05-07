#ifndef BSP_AVR_H
#define BSP_AVR_H

#include <avr/interrupt.h>

#define enter_critical_region() cli()
#define leave_critical_region() sei()

#endif /* BSP_AVR_H */
