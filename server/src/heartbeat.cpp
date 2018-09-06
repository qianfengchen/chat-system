#include "heartbeat.h"

HeartBeat::HeartBeat()
{
    lasttime          = time(NULL);
    sendLastTime      = time(NULL);
}
