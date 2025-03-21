#include "Timer.h"

Timer::Timer()
{
    QueryPerformanceFrequency(&mFreq_);
}

void Timer::Start()
{
    isStart_ = true;
    QueryPerformanceCounter(&mStart_);
}

void Timer::Reset()
{
    mStart_ = {};
    now_ = 0.0;
    isStart_ = false;
}