#include "vcd_mutex.h"

vcd_mutex::vcd_mutex(pthread_mutex_t *mutex)
{
    m_mutex = mutex;
    pthread_mutex_lock(mutex);
}

vcd_mutex::~vcd_mutex()
{
    pthread_mutex_unlock(m_mutex);
}
