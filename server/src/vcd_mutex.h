#ifndef VCD_MUTEX_H
#define VCD_MUTEX_H
#include <pthread.h>

class vcd_mutex
{
public:
    vcd_mutex(pthread_mutex_t *mutex);
    ~vcd_mutex();

private:
    pthread_mutex_t *m_mutex;
};

#endif // VCD_MUTEX_H
