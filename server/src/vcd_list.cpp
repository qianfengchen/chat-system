#include "vcd_list.h"

vcd_list::vcd_list()
{
    pthread_mutex_init(&m_packmutex, NULL);
}

vcd_list::~vcd_list()
{

}

/*数据包*/
char *vcd_list::pack_front()
{
    return m_package.front();
}

char* vcd_list::pack_pop()
{
    vcd_mutex mutex_lock(&m_packmutex);
    char *p = m_package.front();
    m_package.pop_front();
    m_packcount--;
    return p;
}

int vcd_list::getPackSize()
{
    return m_packcount;
}

void vcd_list::pack_push(char *p)
{
    vcd_mutex mutex_lock(&m_packmutex);
    m_package.push_back(p);
    m_packcount++;
}
