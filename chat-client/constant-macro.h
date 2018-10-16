#ifndef CONSTANTMACRO_H
#define CONSTANTMACRO_H

#include <iostream>
using namespace std;

#define SERVER_IP "192.168.0.105"
#define PORT 8884
#define USERLISTSTRLENGTH 10240

#pragma pack(1)

enum cmdIdEnum{ LOGIN = 1, SENDtoONE, SENDtoOTHERS, SENDFILENAME,
                FILERET, SENDFILE, UDPSENDFILE, UDPREGISTER, UDPRESEND, HEARTBEAT, QUIT};

struct baseHandle
{
    int cmdId;
    int userId;
    int length;
    int version;
};

struct loginMessageSend
{
    struct baseHandle loginHead;
    char name[50];
    char passwd[20];
};

struct loginMessageRecv
{
    struct baseHandle loginHead;
    char result[10240];
};

struct messageSend
{
    struct baseHandle head;
    int sayToClientId;
    char msg[1024];
};

struct messageRecv
{
    struct baseHandle head;
    int recvFromWhichId;
    char msg[1024];
};

#endif // CONSTANTMACRO_H
