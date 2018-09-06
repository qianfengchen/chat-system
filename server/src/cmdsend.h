#ifndef CMDSEND_H
#define CMDSEND_H
#include "vcd_mutex.h"
#include "head.h"

class CmdSend
{
public:
    CmdSend();

    /*发送相关*/
    int getPosition();
    void setPosition(int pos);
    void setudpsendflag(int flag);
    char *m_udpcopy(char *p, int length);
    char *m_copy(char *p, int length);
    int startSend(int sockfd);
    char *getsendbuff();
    char *getudpsendbuff();
private:
    int position;
    char *sendbuff;
    int  udpsendFlag;
    char *udpsendbuff;

    pthread_mutex_t m_mutex;
};

#endif // CMDSEND_H
