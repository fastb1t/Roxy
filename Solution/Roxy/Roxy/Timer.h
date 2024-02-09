#ifndef _ROXY_TIMER_H_
#define _ROXY_TIMER_H_

namespace Roxy {

    typedef void (*TIMER_CALLBACK)(void*);

    int CreateTimer(TIMER_CALLBACK callback, int time, void* ptr = 0);
    bool DeleteTimer(int id);

}

#endif // !_ROXY_TIMER_H_
