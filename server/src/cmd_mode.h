#ifndef CMD_MODE_H
#define CMD_MODE_H
#include "user.h"
#include "unloginuser.h"
#include "vcd_list.h"
#include "user_list.h"

class cmd_mode
{
public:
    cmd_mode();
    ~cmd_mode();
    static void *startCmd(void *arg);               //新线程
    int getUdpSockfd();
    User_list* get_vcdListToIO();
    void startDealCmd();

    void setWriteSocketFdFlag(char *pack);   //解析数据包并设置标记
    void m_usleep(int time);

    void start();
    void setHasLoginMapFromRequestMode(map<int, User*> *map);    //I/O模块本地已登录用户链表接口
    void setPackListFromRequestMode(vcd_list *packList);    //I/O模块数据包指针链表接口
    void setUnloginUserListFromAcceptMode(User_list *unloginUserList);

    void sendHeartBeat(map<int, User*>::iterator iter);

private:
    vcd_list    *m_packList; //等同于I/O模块的数据包指针链表
    User_list   *m_vcdLoginList;
    User_list   *m_vcdListToIO;
    map<int, char*> m_udpLoseMap;
    map<int, User*> m_map;  //等同于I/O模块本地已登录用户链表
    map<int, unloginuser*> m_loginMap;
    pthread_t  tid, tid2;
    void       *retval;
    struct sockaddr_in serveraddrUdp;
    int udpsock;
    int num;
    time_t  lastlooptime;
};

#endif // CMD_MODE_H
