#ifndef REQUEST_MODE_H
#define REQUEST_MODE_H

#include "head.h"
#include "vcd_list.h"
#include "accept_mode.h"
#include "user.h"
#include "constant_macro.h"
#include "cmd_mode.h"
#include "user_list.h"

class request_mode
{
public:
    request_mode();
    ~request_mode();

    void startUdpFunc();
    void setUdpFd(int fd);
    static void *startUdp(void *arg);
    static void *startUdpSend(void *arg);
    static void *startRequest(void *arg);               //新线程
    void deleteloginUser(int sockfd);
    void start();
    void startRequestFunc();
    void startUdpSendFunc();
    void epollRead(int sockfd);
    void setVcdListFromCmdMode(User_list *vcdList);   //从监听模块获取公用user链表
    void m_usleep(int time);
    /*本地数据包指针链表的接口*/
    vcd_list *getPackList();

private:
    /*用来接收监听模块user链表的指针*/
    User_list       *m_vcdList;
    /*本地数据包指针链表*/
    vcd_list       *m_packList;
    map<int, User*>  m_map;
    pthread_t      tid, tid2, tid3;
    int epfd;
    int nfds;
    int nread;
    int sockfd;
    struct epoll_event ev, events[11000];//ev用于注册事件,数组用于回传要处理的事件

    int udpsock;
    CmdRecv m_recv;
    int total;
};

#endif // REQUEST_MODE_H
