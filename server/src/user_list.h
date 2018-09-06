#ifndef USER_LIST_H
#define USER_LIST_H
#include "vcd_mutex.h"
#include "user.h"
#include "head.h"
#include "unloginuser.h"

class User_list
{
public:
    User_list();
    /*user类型的公共链表*/
    void vcd_push(User *user);
    int  getSize();
    User *vcd_pop();
    /*未登录用户*/
    void vcd_unloginPush(unloginuser *user);
    int getunloginSize();
    unloginuser *vcd_unloginpop();

private:
    /*user类型的公共链表的变量*/
    int             m_count;
    list<User*>      m_user;
    pthread_mutex_t m_mutex;
    /*未登录用户*/
    int             m_unLoginCount;
    list<unloginuser*> m_unloginuser;
    pthread_mutex_t m_unloginmutex;
};

#endif // USER_LIST_H
