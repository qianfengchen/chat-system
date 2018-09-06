#include "cmdsend.h"

CmdSend::CmdSend()
{
    position = 0;
    sendbuff = new char[10240];
    pthread_mutex_init(&m_mutex, NULL);
    udpsendFlag = 0;
}

void CmdSend::setPosition(int pos)
{
    position += pos;
}

int CmdSend::getPosition()
{
    return position;
}

char* CmdSend::getsendbuff()
{
    return sendbuff;
}

char* CmdSend::getudpsendbuff()
{
    return udpsendbuff;
}

char* CmdSend::m_copy(char *p, int length)
{
    vcd_mutex mutex_lock(&m_mutex);
    memcpy(sendbuff+position, p, length);
    return sendbuff+position;
}

char* CmdSend::m_udpcopy(char *p, int length)
{
    udpsendbuff = new char[1436];
    vcd_mutex mutex_lock(&m_mutex);
    memcpy(udpsendbuff, p, length);
    return udpsendbuff;
}

int CmdSend::startSend(int sockfd)
{
    vcd_mutex mutex_lock(&m_mutex);
    int nread = send(sockfd, sendbuff, position, 0);
    if (nread < 0)
    {
        return -1;
    }
    position -= nread;
    memmove(sendbuff, sendbuff + nread, nread);
    return nread;
}

