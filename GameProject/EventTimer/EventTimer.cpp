#include "EventTimer.h"

#include <imgui.h>

void EventTimer::NewFrame()
{
    events_.clear();
    timers_.clear();
    globalTimer_.Start();
}

void EventTimer::BeginEvent(const std::string& _eventName)
{
    timers_[_eventName].Start();
}

void EventTimer::EndEvent(const std::string& _eventName)
{
    auto now = timers_[_eventName].GetNow();
    events_[_eventName] = static_cast<float>(now);
}

void EventTimer::Measure(const std::string& _eventName, const std::function<void()>& _func)
{
    BeginEvent(_eventName);
    _func();
    EndEvent(_eventName);
}

void EventTimer::EndFrame()
{
    deltaTime_ = static_cast<float>(globalTimer_.GetNow());
    globalTimer_.Reset();
}

void EventTimer::ImGui()
{
    ImGui::Begin("EventTimer");

    for (const auto& event : events_)
    {
        ImGui::Text("%s : %.0fms", event.first.c_str(), event.second * 1000.0f);
        ImGui::ProgressBar(event.second / deltaTime_, ImVec2(0.0f, 0.0f), "");
    }
    ImGui::End();

}
