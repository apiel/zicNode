#ifndef ZIC_UTIL_H
#define ZIC_UTIL_H

#include <stdlib.h>
#include <sys/time.h>

long long timeMs(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

#endif