#include "io.h"
CRITICAL_SECTION CriticalSection;

CIo::CIo()
{

}

void CIo::setUserFromMainwindow(CUser *user)
{
    m_userdata = user;
}

void CIo::Start()
{
    while(1)
    {
        FD_ZERO(&wset);
        FD_ZERO(&rset);
        FD_SET(m_userdata->m_sockfd,  &rset);

        if (m_userdata->haveSendFlag)
        {
            FD_SET(m_userdata->m_sockfd, &wset);
        }

        timeout.tv_sec  = 0;
        timeout.tv_usec = 1000;
        int max = m_userdata->m_sockfd;
        int ret = select(max + 1, &rset, &wset, NULL, &timeout);
        if(ret <= 0)
        {
            continue;
        }

        if (FD_ISSET(m_userdata->m_sockfd, &rset))
        {
            ioctl(m_userdata->m_sockfd, FIONREAD, &nread);
            if (nread == 0)
            {
                cout << "ioctl = 0" << endl;
                exit(0);
            }
            if (nread >= sizeof(baseHandle) || m_userdata->recvHeadFlag)
            {
                /*如果是第一次,则只接收一个头*/
                if (!m_userdata->recvHeadFlag)
                {
                    if (!recv(m_userdata->m_sockfd, m_userdata->get_recv(), sizeof(baseHandle), 0))
                    {
                        perror("recv:");
                        exit(0);
                    }
                    struct baseHandle *tmp = (struct baseHandle *)m_userdata->recvbuff;
                    m_userdata->totalLengthOfData = tmp->length;
                    m_userdata->remainLengthtoRecv = tmp->length - sizeof(baseHandle);
                    m_userdata->recvHeadFlag = true;
                }

                /*如果不是第一次，则接收完其他的数据*/
                if (m_userdata->recvHeadFlag)
                {
                    nread = recv(m_userdata->m_sockfd,
                    m_userdata->recvbuff+16+m_userdata->lengthHasRecv, m_userdata->remainLengthtoRecv, 0);
                    if (nread > 0)
                    {
                        m_userdata->remainLengthtoRecv -= nread;
                        m_userdata->lengthHasRecv += nread;
                        if (m_userdata->remainLengthtoRecv == 0)
                        {
                            m_userdata->recvHeadFlag = false;
                            m_userdata->remainLengthtoRecv = m_userdata->totalLengthOfData;
                            m_userdata->lengthHasRecv = 0;
                            m_userdata->haveReadFlag = true;
                            //m_readList->pack_push(m_userdata->recvbuff);
                        }
                    }else
                    {
                        perror("recv:");
                        exit(0);
                    }
                }
            }
        }

        if (FD_ISSET(m_userdata->m_sockfd, &wset))
        {
            while (m_userdata->remainLengthofSend > 0)
            {
                nread = send(m_userdata->m_sockfd,
                    m_userdata->sendbuff+m_userdata->lengthHasSend, m_userdata->remainLengthofSend, 0);
                if (nread < 0)
                {
                    perror("send error");
                    exit(0);
                }
                m_userdata->lengthHasSend += nread;
                m_userdata->remainLengthofSend -= nread;
            }
            delete m_userdata->sendbuff;
            m_userdata->lengthHasSend = 0;
            m_userdata->haveSendFlag = 0;
        }
    }
}

DWORD WINAPI CIo::start(void *arg)//返回值一定要这样写
{
    CIo *io = (CIo *)arg;
    io->Start();
    return 0;
}

void CIo::ioStart()
{
    InitializeCriticalSection(&CriticalSection);
    HANDLE Handle;
    Handle = CreateThread(NULL, 0, start, this, 0, NULL);
    CloseHandle(Handle);
}
