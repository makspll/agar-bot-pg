#ifndef TIMER_H
#define TIMER_H
#include <time.h>

struct Timer{

    void start(){
        started = true;
        clock_gettime(CLOCK_REALTIME, &time_start);
    }

    long double read_ns(){
        assert(started);

        struct timespec time_end;
        clock_gettime(CLOCK_REALTIME, &time_end);

        return (((double)(time_end.tv_sec - time_start.tv_sec)) * 1e9) + (time_end.tv_nsec - time_start.tv_nsec) ;
    }

    bool started;
    struct timespec time_start;
};

#endif // TIMER_H
