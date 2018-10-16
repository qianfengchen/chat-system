#ifndef CIO_H
#define CIO_H

#include <QThread>

#include "user.h"
#include "constant-macro.h"
#include "savelist.h"

class CIo
{
public:
    CIo();
    ~CIo();
    void ioStart();
    void Start();
    static DWORD WINAPI start(void *arg);
    void setUserFromMainwindow(CUser *user);
    CSaveList *getReadList();

private:
    CUser *m_userdata;
    CSaveList *m_readList;
    fd_set rset;
    fd_set wset;
    struct timeval timeout;
    int nread;
};

#endif // CIO_H
