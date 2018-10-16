#ifndef CIO_H
#define CIO_H

#include <QThread>

#include "user.h"
#include "constant-macro.h"

class CIo
{
public:
    CIo();
    void ioStart();
    void Start();
    static DWORD WINAPI start(void *arg);
    void setUserFromMainwindow(CUser *user);

private:
    CUser *m_userdata;
    fd_set rset;
    fd_set wset;
    struct timeval timeout;
    int nread;
};

#endif // CIO_H
