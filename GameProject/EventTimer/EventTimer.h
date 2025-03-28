#pragma once

#include <Timer/Timer.h>
#include <string>
#include <list>
#include <unordered_map>
#include <functional>

class EventTimer
{
public:
    EventTimer(const EventTimer&) = delete;
    EventTimer& operator=(const EventTimer&) = delete;
    EventTimer(const EventTimer&&) = delete;
    EventTimer& operator=(const EventTimer&&) = delete;

    static EventTimer* GetInstance()
    {
        static EventTimer instance;
        return &instance;
    }

    void NewFrame();
    void BeginEvent(const std::string& _eventName);
    void EndEvent(const std::string& _eventName);

    void Measure(const std::string& _eventName, const std::function<void()>& _func);

    void EndFrame();

    void ImGui();

private:
    EventTimer() = default;
    ~EventTimer() = default;

private:
    Timer globalTimer_;
    float deltaTime_ = 0.0f;
    std::unordered_map<std::string, float> events_;
    std::unordered_map<std::string, Timer> timers_;
};