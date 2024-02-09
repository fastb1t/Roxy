#include "Timer.h"

#include <vector>
#include <windows.h>


struct TimerData {
    int id;
    Roxy::TIMER_CALLBACK callback;
    int time;
    void* ptr;
    bool bExit;
};

static std::vector<TimerData*> g_timers;


// [Wait1]:
static bool Wait1(int interval, bool* bExitFlag)
{
    ULONGLONG dwStartTime = GetTickCount64();

    while (!(*bExitFlag))
    {
        if (GetTickCount64() >= (dwStartTime + interval))
        {
            return true;
        }

        Sleep(1);
    }
    return false;
}
// [/Wait1]


// [Wait2]:
static bool Wait2(int interval, bool* bExitFlag)
{
    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER Frequency;

    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);

    while (!(*bExitFlag))
    {
        QueryPerformanceCounter(&EndingTime);

        ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
        ElapsedMicroseconds.QuadPart *= 1000000;
        ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

        if (ElapsedMicroseconds.QuadPart >= (long long)interval * 1000)
        {
            return true;
        }

        Sleep(1);
    }
    return false;
}
// [/Wait2]


// [Thread]:
static DWORD WINAPI Thread(LPVOID lpObject)
{
    TimerData* data = reinterpret_cast<TimerData*>(lpObject);
    if (!data)
    {
        ExitThread(1);
    }
    else
    {
        while (!data->bExit)
        {
            if (Wait1(data->time - 2, &data->bExit))
            {
                data->callback(data->ptr);
            }
        }

        delete data;

        ExitThread(0);
    }
}
// [/Thread]


// [Roxy::CreateTimer]:
int Roxy::CreateTimer(TIMER_CALLBACK callback, int time, void* ptr)
{
    if (time <= 0)
    {
        return -1;
    }

    TimerData* data = new TimerData;
    if (!data)
    {
        return -1;
    }

    bool bIsBusy;
    for (data->id = 1; ; data->id++)
    {
        bIsBusy = false;
        for (std::vector<TimerData*>::const_iterator it = g_timers.begin(); it != g_timers.end(); it++)
        {
            if ((*it)->id == data->id)
            {
                bIsBusy = true;
                break;
            }
        }

        if (!bIsBusy)
        {
            break;
        }
    }

    data->callback = callback;
    data->time = time;
    data->ptr = ptr;
    data->bExit = false;

    HANDLE hThread = CreateThread(NULL, 0, Thread, reinterpret_cast<LPVOID>(data), 0, NULL);
    if (hThread)
    {
        CloseHandle(hThread);

        g_timers.push_back(data);
        return data->id;
    }

    delete data;
    return -1;
}
// [/Roxy::CreateTimer]


// [Roxy::DeleteTimer]:
bool Roxy::DeleteTimer(int id)
{
    for (std::vector<TimerData*>::iterator it = g_timers.begin(); it != g_timers.end(); it++)
    {
        if ((*it)->id == id)
        {
            (*it)->bExit = true;
            g_timers.erase(it);
            return true;
        }
    }
    return false;
}
// [/Roxy::DeleteTimer]
