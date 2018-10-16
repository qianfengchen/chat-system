#include "savelist.h"

CSaveList::CSaveList()
{
    m_packCount = 0;
}

void CSaveList::packPush(char *p)
{
    QMutexLocker locker(&m_mutex);
    m_package.push_back(p);
    m_packCount++;
}

int CSaveList::getPackSize()
{
    return m_packCount;
}

char *CSaveList::packPop()
{
    QMutexLocker locker(&m_mutex);
    char *p = m_package.front();
    m_package.pop_front();
    m_packCount--;
    return p;
}
