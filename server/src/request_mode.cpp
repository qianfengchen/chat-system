#include "request_mode.h"
#include "accept_mode.h"

request_mode::request_mode()
{
    m_packList = new vcd_list;
    total = 0;
}

request_mode::~request_mode()
{
    delete m_packList;
}

/*数据包指针接口*/
vcd_list *request_mode::getPackList()
{
    return m_packList;
}

void request_mode::setUdpFd(int fd)
{
    udpsock = fd;
}

/*接口链表的接收*/
void request_mode::setVcdListFromCmdMode(User_list *vcdList)
{
    m_vcdList = vcdList;
}

void request_mode::deleteloginUser(int sockfd)
{
    ev.data.fd = sockfd;//设置与要处理的事件相关的文件描述符
    epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);//注册epoll事件
    m_map[sockfd]->deleteFlag2 = true;
    m_map[sockfd]->deleteFlag1 = true;
    m_map.erase(sockfd);
}

void request_mode::epollRead(int sockfd)
{
    /*判断是否满一个头的长度*/
    ioctl(sockfd, FIONREAD, &nread);
    if (nread == 0)
    {
        cout << "ioctl = 0" << endl;
        deleteloginUser(sockfd);
        return;
    }
    if (nread >= sizeof(baseHandle) || m_map[sockfd]->m_recv.recvHeadFlag == 1)
    {
        if (m_map[sockfd]->m_recv.recvHeadFlag == 0)
        {
            if (!recv(sockfd, m_map[sockfd]->m_recv.get_recvbuff(), sizeof(baseHandle), 0))
            {
                deleteloginUser(sockfd);
                return;
            }
            struct baseHandle *tmp = (struct baseHandle *)m_map[sockfd]->m_recv.recvbuff;
            m_map[sockfd]->m_recv.totalLengthOfData = tmp->length;
            m_map[sockfd]->m_recv.lengthHasRecv = sizeof(baseHandle);
            m_map[sockfd]->m_recv.recvHeadFlag = true;
        }
        if (m_map[sockfd]->m_recv.recvHeadFlag == 1)
        {
            int nres = m_map[sockfd]->m_recv.totalLengthOfData - m_map[sockfd]->m_recv.lengthHasRecv;
            nread = recv(sockfd,
                  m_map[sockfd]->m_recv.recvbuff + m_map[sockfd]->m_recv.lengthHasRecv, nres, 0);
            if (nread < 0)
            {
                deleteloginUser(sockfd);
            }
            m_map[sockfd]->m_recv.lengthHasRecv += nread;
            if (m_map[sockfd]->m_recv.lengthHasRecv == m_map[sockfd]->m_recv.totalLengthOfData)
            {
                m_packList->pack_push(m_map[sockfd]->m_recv.recvbuff);
                m_map[sockfd]->m_recv.lengthHasRecv = 0;
                m_map[sockfd]->m_recv.recvHeadFlag = false;
            }
        }
     }
}

void request_mode::startRequestFunc()
{
    int i;
    epfd = epoll_create(11000);//生成用于处理accept的epoll专用的文件描述符

    for (; ; )
    { 
        if (m_vcdList->getSize() > 0)
        {
            User *user = m_vcdList->vcd_pop();
            m_map.insert(pair<int, User*>(user->get_socket(), user));/*用户ID和套接字一一对应插入map中*/

            ev.data.fd = user->get_socket();//设置用于写操作的文件描述符
            ev.events = EPOLLIN;//设置用于注测的读操作事件
            epoll_ctl(epfd, EPOLL_CTL_ADD, user->get_socket(), &ev);//注册ev
        }

        if (m_map.size() > 0)
        {
            map<int, User*>::iterator iter = m_map.begin();
            while (iter != m_map.end())
            {

                if (iter->second->m_send->getPosition() > 0)
                {
                    ev.data.fd = iter->first;//设置用于写操作的文件描述符
                    ev.events = EPOLLOUT;//设置用于注测的写操作事件
                    epoll_ctl(epfd, EPOLL_CTL_MOD, iter->first, &ev);//修改sockfd上要处理的事件为EPOLLOUT
                }

                /*判断有没有正常退出的*/
                if (iter->second->deleteFlag1)
                {
                    deleteloginUser(iter->first);
                }
                iter++;
            }
        }

        nfds = epoll_wait(epfd, events, 11000, 100);
        //处理所发生的所有事件
        for (i=0; i<nfds; ++i)
        {
          if (events[i].events&EPOLLIN)
          {
              if ((sockfd = events[i].data.fd) < 0)
              {
                  continue;
              }
              epollRead(sockfd);
         }
         else if (events[i].events&EPOLLOUT)
         {
              if ((sockfd = events[i].data.fd) < 0)
              {
                  continue;
              }
              nread = m_map[sockfd]->m_send->startSend(sockfd);
              if (nread == -1)
              {
                  deleteloginUser(sockfd);
              }
              ev.data.fd = sockfd;//设置用于写操作的文件描述符
              ev.events = EPOLLIN;//设置用于注测的读操作事件
              epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);//注册ev
         }
      }
   }
}

void request_mode::m_usleep(int time)
{
    fd_set rset;
    FD_ZERO(&rset);
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void request_mode::startUdpFunc()
{
    socklen_t tmp_clientlen = sizeof(struct sockaddr_in);
    for ( ; ;)
    {
        struct sockaddr_in *tmp_clientaddrUdp = new (struct sockaddr_in);
        nread = recvfrom(udpsock, m_recv.get_recvbuff(), 1436, 0,
                (struct sockaddr *)tmp_clientaddrUdp, &tmp_clientlen);
        if (nread < 0)
        {
            cout << "recvfrom error" << endl;
            return;
        }
        m_packList->pack_push(m_recv.recvbuff);
        m_packList->pack_push((char *)tmp_clientaddrUdp);
    }
}

void request_mode::startUdpSendFunc()
{
    char *pack = NULL;
    int flag = 0;
    for( ; ;)
    {
        if (m_map.size() > 0)
        {
            map<int, User*>::iterator iter = m_map.begin();
            while (iter != m_map.end())
            {
                flag +=  iter->second->m_udpSendList->getPackSize();
                if ( iter->second->m_udpSendList->getPackSize() > 0)
                {
                    pack = iter->second->m_udpSendList->pack_pop();
                    sendto(udpsock, pack, 1436, 0, (struct sockaddr *) iter->second->clientaddrUdp,
                             iter->second->clientlen);
                    delete pack;
                    pack = NULL;
                }
                iter++;
            }
            if (flag == 0)
            {
                m_usleep(10000);
            }else
            {
                m_usleep(40);
            }
            flag = 0;
        }
        else
        {
            m_usleep(10000);
        }
    }
}


/*开启线程函数*/
void *request_mode::startRequest(void *arg)
{
    request_mode *request = (request_mode *)arg;
    request->startRequestFunc();
}

void *request_mode::startUdp(void *arg)
{
    request_mode *udp = (request_mode *)arg;
    udp->startUdpFunc();
}

void *request_mode::startUdpSend(void *arg)
{
    request_mode *udp = (request_mode *)arg;
    //udp->startUdpSendFunc();
}

/*模块启动函数*/
void request_mode::start()
{
    int ret;
    ret = pthread_create(&tid, NULL, startRequest, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }

    ret = pthread_create(&tid2, NULL, startUdp, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }

    ret = pthread_create(&tid3, NULL, startUdpSend, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
