#include "user.h"

CUser::CUser()
{
    haveSendFlag      = false;
    userSendDataFlag  = false;
    lengthHasSend     = 0;

    recvHeadFlag      = false;
    totalLengthOfData = 0;
    lengthHasRecv     = 0;
    haveReadFlag      = false;
}


char *CUser::get_recv()
{
    recvbuff = new char[1436];
    return recvbuff;
}

char *CUser::get_send()
{
    sendbuff = new char[1436];
    return sendbuff;
}


