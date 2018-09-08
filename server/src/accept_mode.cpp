#include "accept_mode.h"

accept_mode::accept_mode()
{
    /*公用接口*/
    m_vcdListtoIO  = new vcd_list;
    m_vcdLoginList = new User_list;
    /*用户ID初始化为1，以后不断垒加*/
    userId = 1;

    /*长度初始化*/
    clilen = sizeof(clientaddr);
}

accept_mode::~accept_mode()
{
    /*关闭监听套接字*/
    close(listenfd);

    /*关闭epoll套接字*/
    close(epfd);

    /*删除公用链表*/
    delete m_vcdListtoIO;
    delete m_vcdLoginList;
}

/*登录模块公用链表接口*/
vcd_list *accept_mode::IOGetVcdList()
{
    return m_vcdListtoIO;
}

User_list *accept_mode::getVcdLoginList()
{
    return m_vcdLoginList;
}

void accept_mode::deleteOneUnloginUser(int sockfd)
{
    m_loginMap[sockfd]->deleteFlag1 = true;
    m_loginMap[sockfd]->deleteFlag2 = true;
    ev.data.fd = sockfd;//设置与要处理的事件相关的文件描述符
    epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);//注册epoll事件
    m_loginMap.erase(sockfd);
}

/*获取当前时间和本套接字连接时的时间相比，超过一分钟，表示登录出错，删除它，并返回错误信息*/
void accept_mode::timeoutFunc(int sockfd)
{
    time_t tmpTime = time(NULL);
    if (tmpTime - m_loginMap[sockfd]->loginTime > 60)
    {
        /*设置错误返回信息*/
        strcpy(m_loginMap[sockfd]->m_loginMsgSend.result, "N");
        send(m_loginMap[sockfd]->m_socket, &m_loginMap[sockfd]->m_loginMsgSend, sizeof(loginMessageSend), 0);
        deleteOneUnloginUser(connfd);
    }
}

/*副线程主函数*/
void accept_mode::func_accept ()
{
    int i;
    epfd = epoll_create(11000);//生成用于处理accept的epoll专用的文件描述符
    ev.data.fd = listenfd;//设置与要处理的事件相关的文件描述符
    ev.events = EPOLLIN|EPOLLET;//设置要处理的事件类型
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);//注册epoll事件
    for (; ; )
    {
        map<int, unloginuser*>::iterator iter = m_loginMap.begin();

        while (iter != m_loginMap.end())
        {
            ev.data.fd = iter->first;//设置用于写操作的文件描述符
            if (iter->second->loginState != NOLOGIN)
            {
                ev.data.fd = iter->first;//设置与要处理的事件相关的文件描述符
                ev.events = EPOLLOUT;//设置用于注测的写操作事件
                epoll_ctl(epfd, EPOLL_CTL_MOD, iter->first, &ev);//修改sockfd上要处理的事件为EPOLLOUT
            }
            iter++;
        }
          nfds = epoll_wait(epfd, events, 11000, 1);
          //处理所发生的所有事件
          for (i=0; i<nfds; ++i)
          {
              if (events[i].data.fd == listenfd)//事件发生在listenfd上
              {
                  connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clilen);
                  if (connfd < 0)
                  {
                      perror("connfd < 0");
                      continue;
                  }
                  fcntl(connfd, F_SETFL, O_NONBLOCK);

                  cout << "有套接字连接：" << "socket=" << connfd << "userId=" << userId << endl;

                  ev.data.fd = connfd;//设置用于读操作的文件描述符
                  ev.events = EPOLLIN;//设置用于注测的读操作事件
                  epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);//注册ev
                  unloginuser *user;
                  user = new unloginuser;
                  user->m_socket = connfd;
                  user->userId   = userId;
                  user->loginTime = time(NULL);           /*记录连接的时间，防止连接超时*/
                  m_vcdLoginList->vcd_unloginPush(user);
                  m_loginMap.insert(pair<int, unloginuser*>(connfd, user));

              }
              else if (events[i].events&EPOLLIN)
              {
                  if ((connfd = events[i].data.fd) < 0)
                  {
                      continue;
                  }
                  /*判断recv底层buff是否够一个头的长度*/
                  ioctl(connfd, FIONREAD, &nread);
                  if (nread == 0)
                  {
                      deleteOneUnloginUser(connfd);
                  }
                  if (nread >= sizeof(baseHandle) || !m_loginMap[connfd]->recvHeadFlag)
                  {
                      cout << "socket--" << connfd << " --发来登录信息" << endl;
                      /*如果第一次，则只接收一个头的长度*/
                      if (m_loginMap[connfd]->recvHeadFlag)
                      {
                          if (!recv(connfd, m_loginMap[connfd]->recvbuff, sizeof(baseHandle), 0))
                          {
                               deleteOneUnloginUser(connfd);
                               continue;
                          }
                          struct loginMessageRecv *tmp = (struct loginMessageRecv *)m_loginMap[connfd]->recvbuff;
                          /*获取头以后解析并存储整个包的长度*/
                          if (tmp->loginHead.cmdId == LOGIN)
                          {
                              m_loginMap[connfd]->totalLengthOfData = tmp->loginHead.length;
                              m_loginMap[connfd]->remainLengthofRecv = tmp->loginHead.length - sizeof(baseHandle);
                              m_loginMap[connfd]->recvHeadFlag = false;
                          }else
                          {
                               deleteOneUnloginUser(connfd);
                          }
                      }

                      /*如果不是第一次，则接收头以外的所有数据*/
                      if (!m_loginMap[connfd]->recvHeadFlag)
                      {
                          nread = recv(m_loginMap[connfd]->m_socket, m_loginMap[connfd]->recvbuff+sizeof(baseHandle),
                                  m_loginMap[connfd]->remainLengthofRecv, 0);
                          if (nread > 0)
                          {
                              m_loginMap[connfd]->remainLengthofRecv -= nread;
                              if (m_loginMap[connfd]->remainLengthofRecv == 0)
                              {
                                  m_loginMap[connfd]->recvHeadFlag = true;
                              }
                          }else
                          {
                              deleteOneUnloginUser(connfd);
                          }
                      }
                  }else
                  {
                      /*获取当前时间和本套接字连接时的时间相比，超过一分钟，表示登录出错，删除它，并返回错误信息*/
                      timeoutFunc(connfd);
                  }
              }
              else if ((events[i].events&EPOLLOUT))
              {
                  int nread;
                  connfd = events[i].data.fd;
                  /*return*/
                  nread = send(connfd, &m_loginMap[connfd]->m_loginMsgSend, sizeof(loginMessageSend), 0);
                  if (nread == 0)
                  {
                      deleteOneUnloginUser(connfd);
                      continue;
                  }
                  m_loginMap[connfd]->remainSend -= nread;
                  if (m_loginMap[connfd]->remainSend == 0)
                  {
                      ev.data.fd = connfd;//设置与要处理的事件相关的文件描述符
                      epoll_ctl(epfd, EPOLL_CTL_DEL, connfd, &ev);//注册epoll事件
                      if (m_loginMap[connfd]->loginState == LOGINSUCCESS)
                      {
                          cout << "userId++" << endl;
                          userId++;/*用户ID自加1,分给下个用户*/
                      }
                      m_loginMap[connfd]->loginReturnFlag = true;
                      /*将用户从未登录链表中删除*/
                      m_loginMap.erase(connfd);
                  }
              }
         }
    }
}

void *accept_mode::startAccept(void *arg)
{
    accept_mode *acc = (accept_mode *)arg;
    acc->func_accept();
}

/*主线程启动*/
void accept_mode::start()
{
    struct sockaddr_in servaddr;
    int ret;
    int flag = 1;
    int socket_len = sizeof(int);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("socket init failed");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_aton(IP, &(servaddr.sin_addr));
    servaddr.sin_port = htons(PORT);

    /*设置socket关闭后能马上继续使用*/
    ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, socket_len);
    if( ret == -1)
    {
        perror("setsockopt");
    }

    /*绑定*/
    ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(ret < 0)
    {
        perror("bind fail");
        exit(1);
    }

    /*监听*/
    ret = listen(listenfd, LISTENNUM);
    if(ret < 0)
    {
        perror("listen fail");
        exit(1);
    }

    /*创建新线程接收新客户端套接字*/
    ret = pthread_create(&tid, NULL, startAccept, this);
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
