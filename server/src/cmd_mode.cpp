#include "cmd_mode.h"

cmd_mode::cmd_mode()
{
    m_vcdListToIO = new User_list;
    /********初始化UDP套接字**********/
    udpsock = socket(AF_INET,SOCK_DGRAM,0);//UDP传输套接字创建
    bzero(&serveraddrUdp,0);
    serveraddrUdp.sin_family = AF_INET;//sin_family，协议族内一员
    serveraddrUdp.sin_port = htons(8888);//端口号，网络字节序,要转换成大端模式
    inet_aton("127.0.0.1",&(serveraddrUdp.sin_addr));//ip地址号，网络字节序，十进制与32位地址之间的转换
    bind(udpsock, (struct sockaddr *)(&serveraddrUdp), sizeof(serveraddrUdp));//绑定套节字并强转成struct sockaddr *型
    /******************************/
    num = 0;
}

cmd_mode::~cmd_mode()
{
    delete m_vcdListToIO;
}

User_list *cmd_mode::get_vcdListToIO()
{
    return m_vcdListToIO;
}

/*获取I/O模块的数据包指针链表*/
void cmd_mode::setPackListFromRequestMode(vcd_list *packList)
{
    m_packList = packList;
}

void cmd_mode::setUnloginUserListFromAcceptMode(User_list *unloginUserList)
{
    m_vcdLoginList = unloginUserList;
}

int cmd_mode::getUdpSockfd()
{
    return udpsock;
}

void cmd_mode::m_usleep(int time)
{
    fd_set rset;
    FD_ZERO(&rset);
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = time;
    select(1, &rset, NULL, NULL, &timeout);
}

void cmd_mode::sendHeartBeat(map<int, User*>::iterator iter)
{
    iter->second->m_recv.get_recvbuff();
    iter->second->m_recv.m_msgRecv = (struct messageRecv *)iter->second->m_recv.recvbuff;
    strcpy(iter->second->m_recv.m_msgRecv->msg, "heartbeat");
    iter->second->m_recv.m_msgRecv->whichClientIdToSend = iter->first;
    iter->second->m_recv.m_msgRecv->head.cmdId  = SENDtoONE;
    iter->second->m_recv.m_msgRecv->head.length = 30;
    iter->second->m_recv.m_msgRecv->head.userId = 0;
    m_packList->pack_push(iter->second->m_recv.recvbuff);
    iter->second->m_heart.sendLastTime = time(NULL);
}

/*判断哪个用户的套接字需要设置进写集合，通过flag设置*/
void cmd_mode::setWriteSocketFdFlag(char *pack)
{
    char *resend2    = NULL;
    char *resend     = NULL;
    char *sendpack   = NULL;
    char *clientaddr = NULL;
    char *temaddr    = NULL;
    struct messageRecv *tmp;
    struct fileRecv    *fileTmp;
    struct fileSend    *sfileTmp;
    struct udpfileRecv *udpfileTmp;
    struct udpfileSend *udpsfileTmp;
    struct messageSend *stmp;
    struct udpLoseNumRecv *loseTmpRecv;
    struct udpLoseNumSend *loseTmpSend;
    struct baseHandle  *head = (struct baseHandle *)pack;
    map<int, User*>::iterator iter = m_map.begin();
    map<int, char*>::iterator udpiter;
    int num = 0;
    switch(head->cmdId)
    {
        case UDPRESEND:
            if (m_packList->getPackSize() > 0)
            {
                temaddr = m_packList->pack_pop();
                delete temaddr;
            }
            loseTmpRecv = (struct udpLoseNumRecv *)pack;
            for (int i=0; i<loseTmpRecv->num; i++)
            {
                udpiter = m_udpLoseMap.find(loseTmpRecv->loseNum[i]);
                if(udpiter != m_udpLoseMap.end())
                {
                    resend = new char[1436];
                    memcpy(resend, m_udpLoseMap[loseTmpRecv->loseNum[i]], 1436);
                    m_map[loseTmpRecv->head.userId]->m_udpSendList->pack_push(resend);
                }else
                {
                    if (num == 0)
                    {
                        resend2 = new char[220];
                        loseTmpSend = (struct udpLoseNumSend *)resend2;
                        loseTmpSend->recvFromWhichClient = loseTmpRecv->head.userId;
                    }
                    loseTmpSend->loseNum[num] = loseTmpRecv->loseNum[i];
                    num++;
                }
            }
            if (num != 0)
            {
                loseTmpSend->num = num;
                loseTmpSend->head.cmdId = UDPRESEND;
                m_map[loseTmpRecv->whichClientIdToSend]->m_udpSendList->pack_push(resend2);
                num = 0;
            }
            delete pack;
            pack = NULL;
        break;

        /*私聊*/
        case SENDtoONE:
            cout << "私聊" << endl;
            tmp = (struct messageRecv *)pack;
            stmp = (struct messageSend *)m_map[tmp->whichClientIdToSend]->m_send->m_copy(pack, tmp->head.length);
            if (tmp->head.userId == 0)
            {
                stmp->head.cmdId = HEARTBEAT;
            }else
            {
                stmp->recvFromWhichClient = tmp->head.userId;
            }
            m_map[tmp->whichClientIdToSend]->m_send->setPosition(tmp->head.length);
            delete pack;
            pack = NULL;
        break;

        /*群聊*/
        case SENDtoOTHERS:
            cout << "群聊" << endl;
            tmp = (struct messageRecv *)pack;
            while (iter != m_map.end())
            {
                stmp = (struct messageSend *)iter->second->m_send->m_copy(pack, tmp->head.length);
                stmp->recvFromWhichClient = tmp->head.userId;
                iter->second->m_send->setPosition(tmp->head.length);
                iter++;
            }
            delete pack;
            pack = NULL;
        break;

        /*文件*/
        case SENDFILENAME:
            cout << "发文件名" << endl;
            tmp = (struct messageRecv *)pack;
            stmp = (struct messageSend *)m_map[tmp->whichClientIdToSend]->m_send->m_copy(pack, tmp->head.length);
            stmp->recvFromWhichClient = tmp->head.userId;
            m_map[tmp->whichClientIdToSend]->m_send->setPosition(tmp->head.length);
            delete pack;
            pack = NULL;
        break;

        /*接收文件返回*/
        case FILERET:
            cout << "文件返回" << endl;
            tmp = (struct messageRecv *)pack;
            stmp = (struct messageSend *)m_map[tmp->whichClientIdToSend]->m_send->m_copy(pack, tmp->head.length);
            stmp->recvFromWhichClient = tmp->head.userId;
            m_map[tmp->whichClientIdToSend]->m_send->setPosition(tmp->head.length);
            delete pack;
            pack = NULL;
        break;

        /*发文件*/
        case SENDFILE:
            fileTmp = (struct fileRecv *)pack;
            sfileTmp = (struct fileSend *)m_map[fileTmp->sendToClientId]->m_send->m_copy(pack, fileTmp->head.length);
            sfileTmp->recvFromWhichId = fileTmp->head.userId;
            m_map[fileTmp->sendToClientId]->m_send->setPosition(fileTmp->head.length);
            delete pack;
            pack = NULL;
        break;

        case UDPSENDFILE:
            if (m_packList->getPackSize() > 0)
            {
                temaddr = m_packList->pack_pop();
                delete temaddr;
            }
            udpfileTmp = (struct udpfileRecv *)pack;
            sendpack = m_map[udpfileTmp->sendToClientId]->m_send->m_udpcopy(pack, udpfileTmp->head.length);
            udpsfileTmp = (struct udpfileSend *)sendpack;
            udpsfileTmp->recvFromWhichId = udpfileTmp->head.userId;
            m_map[udpfileTmp->sendToClientId]->m_udpSendList->pack_push(sendpack);
            udpfileTmp->time = time(NULL);
            m_udpLoseMap.insert(pair<int, char*>(udpfileTmp->count, pack));
        break;

        case UDPREGISTER:
            fileTmp = (struct fileRecv *)pack;
            cout << head->userId << "注册成功" << endl;
            clientaddr = m_packList->pack_pop();
            m_map[head->userId]->clientaddrUdp = (struct sockaddr_in *)(clientaddr);
            delete pack;
            pack = NULL;
        break;

        /*心跳*/
        case HEARTBEAT:
            cout << "心跳包" << endl;
            tmp = (struct messageRecv *)pack;
            m_map[tmp->head.userId]->m_heart.lasttime = time(NULL);
            delete pack;
            pack = NULL;
            break;

        case QUIT:
            m_map[tmp->head.userId]->deleteFlag1 = true;
            delete pack;
            pack = NULL;
        break;
    }
}

/*线程启动函数*/
void cmd_mode::startDealCmd()
{
    char *pack = NULL;
    struct timeval currenttime;
    struct timeval lasttime;
    gettimeofday(&lasttime,NULL);
    lastlooptime = time(NULL);
    for (; ; )
    {
        gettimeofday(&currenttime,NULL);
        if(currenttime.tv_sec-lasttime.tv_sec > 1)
        {
            gettimeofday(&lasttime,NULL);
            if (m_udpLoseMap.size() > 0)
            {
                map<int, char*>::iterator iter = m_udpLoseMap.begin();
                struct udpfileRecv *tmp = NULL;
                while (iter != m_udpLoseMap.end())
                {
                    tmp = (struct udpfileRecv *)iter->second;
                    if ((time(NULL) - tmp->time) > 2)
                    {
                        delete iter->second;
                        iter->second = NULL;
                        m_udpLoseMap.erase(iter);
                    }
                    iter++;
                }
            }
        }

        if (m_packList->getPackSize() > 0)
        {
            pack = m_packList->pack_pop();
            setWriteSocketFdFlag(pack);
            m_usleep(40);
        }else
        {
            m_usleep(1000);
        }

        if (m_vcdLoginList->getunloginSize() > 0)
        {
            unloginuser *user = m_vcdLoginList->vcd_unloginpop();
            m_loginMap.insert(pair<int, unloginuser*>(user->m_socket, user));
        }

        if (time(NULL) - lastlooptime > 1)
        {
            if (m_map.size() > 0)
            {
                map<int, User*>::iterator iter = m_map.begin();
                while (iter != m_map.end())
                {
                    /*判断客户端发国来的心跳包有没有超时,超时则删除*/
                    if (time(NULL) - iter->second->m_heart.lasttime > 40)
                    {
                        //iter->second->deleteFlag1 = true;
                    }

                    /*定时发送心跳包*/
                    if (time(NULL)-iter->second->m_heart.sendLastTime > 20)
                    {
                        //sendHeartBeat(iter);
                    }

                    /*删除*/
                    if (iter->second->deleteFlag1 && iter->second->deleteFlag2)
                    {
                        close(iter->second->get_socket());
                        delete(iter->second);
                        iter->second = NULL;
                        m_map.erase(iter);
                    }
                    iter++;
                }
            }
            lastlooptime = time(NULL);
        }
        if (m_loginMap.size() > 0)
        {
            map<int, unloginuser*>::iterator loginiter = m_loginMap.begin();
            while (loginiter != m_loginMap.end())
            {
                loginiter->second->m_loginMsgRecv = (struct loginMessageRecv *)loginiter->second->recvbuff;
                if(strcmp(loginiter->second->m_loginMsgRecv->name, "aa") == 0
                        && strcmp(loginiter->second->m_loginMsgRecv->passwd, "aa") == 0)
                {
                    /*认证成功*/
                    cout << "用户名密码认证成功" << endl;
                    loginiter->second->loginSuccess = true;
                    strcpy(loginiter->second->m_loginMsgSend.result, "Y");
                    loginiter->second->m_loginMsgSend.loginHead.userId = loginiter->second->userId;
                }
                if (loginiter->second->deleteFlag1 && loginiter->second->deleteFlag2)
                {
                    close(loginiter->first);
                    delete(loginiter->second);
                    loginiter->second = NULL;
                    m_loginMap.erase(loginiter);
                }
                if (loginiter->second->loginReturnFlag)
                {
                    User *user;
                    user = new User;
                    user->set_socket(loginiter->first);
                    user->set_userId(loginiter->second->userId);

                    m_map.insert(pair<int, User*>(loginiter->second->userId, user));
                    cout << "有一个正式用户" << endl;
                    m_vcdListToIO->vcd_push(user);

                    delete loginiter->second;
                    loginiter->second = NULL;
                    m_loginMap.erase(loginiter);
                }
                loginiter++;
            }
        }
    }
}

void *cmd_mode::startCmd(void *arg)
{
    cmd_mode *cmd = (cmd_mode *)arg;
    cmd->startDealCmd();
}

/*模块启动*/
void cmd_mode::start()
{
    int ret;
    ret = pthread_create(&tid, NULL, startCmd, this); //创建线程
    if (ret != 0)
    {
        cout << "pthread error" << endl;
        exit(1);
    }
}
