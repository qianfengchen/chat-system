#include <iostream>
#include "accept_mode.h"
#include "request_mode.h"
#include "cmd_mode.h"
using namespace std;

int main()
{
    accept_mode  acceptMode;
    request_mode requestMode;
    cmd_mode     cmdMode;


    /*接口传递*/
    requestMode.setVcdListFromCmdMode(cmdMode.get_vcdListToIO());
    requestMode.setUdpFd(cmdMode.getUdpSockfd());
    cmdMode.setUnloginUserListFromAcceptMode(acceptMode.getVcdLoginList());
    cmdMode.setPackListFromRequestMode(requestMode.getPackList()); //指令模块获取I/O模块数据包指针链表

    /*各模块启动*/
    acceptMode.start();  //监听
    requestMode.start(); //I/O
    cmdMode.start();     //指令

    /*退出*/
    cout << "按任意键退出" << endl;
    char quit;
    cin >> quit;

    return 0;
}
