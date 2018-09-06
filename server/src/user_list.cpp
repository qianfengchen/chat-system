#include "user_list.h"

User_list::User_list()
{
    pthread_mutex_init(&m_mutex, NULL);
    pthread_mutex_init(&m_unloginmutex, NULL);
}

/*登录用户user链表*/
User *User_list::vcd_pop()
{
    vcd_mutex mutex_lock(&m_mutex);
    User *user = m_user.front();
    m_user.pop_front();
    m_count--;
    return user;
}

int User_list::getSize()
{
    return m_count;
}

void User_list::vcd_push(User *user)
{
    vcd_mutex mutex_lock(&m_mutex);
    m_user.push_back(user);
    m_count++;
}
/*未登录用户*/
unloginuser *User_list::vcd_unloginpop()
{
    vcd_mutex mutex_lock(&m_unloginmutex);
    unloginuser *user = m_unloginuser.front();
    m_unloginuser.pop_front();
    m_unLoginCount--;
    return user;
}

int User_list::getunloginSize()
{
    return m_unLoginCount;
}

void User_list::vcd_unloginPush(unloginuser *user)
{
    vcd_mutex mutex_lock(&m_unloginmutex);
    m_unloginuser.push_back(user);
    m_unLoginCount++;
}
