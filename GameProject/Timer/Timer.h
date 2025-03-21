#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Timer
{
public:
    Timer();
    ~Timer() {};

    void Start();
    void Reset();
    template <typename T>
    T GetNow();

    bool GetIsStart() const { return isStart_; }

private:
    LARGE_INTEGER mFreq_    = {};
    LARGE_INTEGER mStart_   = {};
    double now_             = 0.0;
    bool isStart_           = false;
};

template <typename T>
inline T Timer::GetNow()
{
    if (!isStart_) return T();

    LARGE_INTEGER mNow = {};
    QueryPerformanceCounter(&mNow);

    now_ = static_cast<double>(mNow.QuadPart - mStart_.QuadPart) / static_cast<double>(mFreq_.QuadPart);

    return static_cast<T>(now_);
}
