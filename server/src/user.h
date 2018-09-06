#ifndef USER_H
#define USER_H
#include "head.h"
#include "constant_macro.h"
#include "cmdrecv.h"
#include "cmdsend.h"
#include "heartbeat.h"
#include "vcd_list.h"
using namespace std;

class User
{
public:
    User();
    ~User();
    void set_socket(int socket);
    void set_userId(int userId);
    int  get_socket();
    int  get_userId();

    /*关于删除*/
    bool deleteFlag1;
    bool deleteFlag2;

    /*每个用户udp发送的list*/
    vcd_list *m_udpSendList;

    /*关于心跳*/
    HeartBeat m_heart;

    /*发送相关*/
    CmdSend  *m_send;

    /*接收相关*/
    CmdRecv m_recv;

    /*UDP*/
    struct sockaddr_in *clientaddrUdp;
    socklen_t clientlen;

private:

    int  m_socket;      /*本用户套接字*/
    int  m_userId;      /*用户id*/
};

#endif // USER_H
