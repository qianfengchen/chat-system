#ifndef CONSTANTMACRO_H
#define CONSTANTMACRO_H


#define SERVER_IP "192.168.0.104"
#define PORT 8884

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
    char result[2];
};

#endif // CONSTANTMACRO_H
