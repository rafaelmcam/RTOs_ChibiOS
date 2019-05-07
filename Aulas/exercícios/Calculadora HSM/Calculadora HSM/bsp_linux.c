#include <pthread.h>
#include "bsp_linux.h"


pthread_mutex_t _event_mutex;

void bsp_init(void)
{
        pthread_mutex_init(&_event_mutex, 0);
}


void enter_critical_region()
{
        pthread_mutex_lock(&_event_mutex);
}


void leave_critical_region()
{
        pthread_mutex_unlock(&_event_mutex);
}
