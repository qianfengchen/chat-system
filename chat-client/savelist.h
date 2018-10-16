#ifndef CSAVELIST_H
#define CSAVELIST_H

#include <list>
#include <QMutex>
#include "constant-macro.h"

class CSaveList
{
public:
    CSaveList();

    void packPush(char *p);
    int getPackSize();
    char *packPop();

private:
    int m_packCount;
    list<char *> m_package;
    QMutex m_mutex;
};

#endif // CSAVELIST_H
