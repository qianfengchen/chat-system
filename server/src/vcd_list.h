#ifndef VCD_LIST_H
#define VCD_LIST_H
#include <iostream>

#include "head.h"
#include "vcd_mutex.h"
#include "cmdsend.h"
#include "constant_macro.h"
using namespace std;
class vcd_list
{
public:
    vcd_list();
    ~vcd_list();

    /*数据包指针的公共链表*/
    void pack_push(char *p);
    int  getPackSize();
    char *pack_pop();
    char *pack_front();

    /*发送*/
    void vcd_sendPush(char *p);
    int getSendSize();
    char *vcd_sendpop();

    /*数据包指针的公共链表的变量*/
    int             m_packcount;
    list<char *>    m_package;
    pthread_mutex_t m_packmutex;

    /*发送*/
    int             m_sendCount;
    list<char*> m_sendList;
    pthread_mutex_t m_sendmutex;
};

#endif // VCD_LIST_H
