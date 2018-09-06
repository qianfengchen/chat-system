#ifndef HEARTBEAT_H
#define HEARTBEAT_H
#include "head.h"

class HeartBeat
{
public:
    HeartBeat();
    time_t lasttime;
    time_t sendLastTime;
};

#endif // HEARTBEAT_H
