#include "user.h"
using namespace std;

User::User()
{
    deleteFlag1       = false;
    deleteFlag2       = false;
    clientlen = sizeof(struct sockaddr_in);
    m_send = new CmdSend;
    m_udpSendList = new vcd_list;
}

User::~User()
{
    delete m_send;
    delete m_udpSendList;
}

void User::set_socket(int socket)
{
    m_socket = socket;
}

void User::set_userId(int userId)
{
    m_userId = userId;
}

int User::get_userId()
{
    return m_userId;
}

int User::get_socket()
{
    return m_socket;
}
