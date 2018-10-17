#ifndef CUSER_H
#define CUSER_H

#include <winsock2.h>
#include "constant-macro.h"

class CUser
{

public:
    CUser();

    /*登录信息*/
    struct loginMessageRecv m_loginRecvMsg;
    struct loginMessageSend m_loginSendMsg;

    int m_userId;
    SOCKET m_sockfd;
    SOCKADDR_IN m_servaddr;

    /*接收*/
    bool haveReadFlag;
    bool recvHeadFlag;                      /*接收头还是数据的标志为*/
    char *recvbuff;
    char *get_recv();
    struct messageRecv *m_MsgRecv;
    int  totalLengthOfData;                 /*数据包的总长度*/
    int  remainLengthtoRecv;                /*剩下需要接收的长度*/
    int  lengthHasRecv;                     /*已经接收的长度*/
    struct userList *m_userList;

    /*发送*/
    bool haveSendFlag;
    char *sendbuff;
    char *get_send();
    struct messageSend *m_MsgSend;
    bool userSendDataFlag;                  /*判断是否需要将其套接字设置进入集合的标志为*/
    int  totalLengthOfSend;                 /*数据包的总长度*/
    int  lengthHasSend;                     /*还需要发送的长度*/
    int  remainLengthofSend;

};

#endif // CUSER_H
