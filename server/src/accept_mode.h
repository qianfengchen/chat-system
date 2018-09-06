#ifndef ACCEPT_MODE_H
#define ACCEPT_MODE_H

#include "head.h"
#include "constant_macro.h"
#include "vcd_list.h"
#include "vcd_mutex.h"
#include "user.h"
#include "unloginuser.h"
#include "user_list.h"

class accept_mode
{
public:
    accept_mode();
    ~accept_mode();

    /*副线程函数*/
    static void *startAccept(void *arg);    //新线程
    void func_accept();                     //主线程函数
    void timeoutFunc(int sockfd);           //超时判断函数
    void deleteOneUnloginUser(int sockfd);

    /*主线程函数*/
    vcd_list *IOGetVcdList();
    User_list *getVcdLoginList();
    void start();

private:
    /*副线程变量*/
    map<int, unloginuser*> m_loginMap; //未登录用户链表
    vcd_list   *m_vcdListtoIO;
    User_list   *m_vcdLoginList;
    int        connfd;                 //新连接的客户端套接字
    int        userId;                 //分配给客户端的用户ID

    /*主副线程公用变量--监听套接字*/
    int        listenfd;
    /*主线程变量*/
    void       *retval;
    pthread_t  tid;
    int epfd;
    int nfds;
    int nread;
    struct sockaddr_in clientaddr;
    socklen_t clilen;
    struct epoll_event ev, events[11000];//ev用于注册事件,数组用于回传要处理的事件

    /********/
};

#endif // ACCEPT_MODE_H
