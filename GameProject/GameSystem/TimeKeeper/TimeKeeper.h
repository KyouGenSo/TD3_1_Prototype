#pragma once

#include <Timer/Timer.h>
#include <string>
#include <unordered_map>
#include <GlobalVariables.h>

class TimeKeeper
{
private:
    struct TimeData
    {
        Timer timer;
        float duration;
    };

public:
    TimeKeeper() = default;
    ~TimeKeeper() = default;

    void Initialize();
    void Update();
    void Run(const std::string& _name);
    void AddEvent(const std::string& _name, float _duration);
    void Load();
    float GetRemainTime(const std::string& _name) 
    { 
        return event_[_name].duration - event_[_name].timer.GetNow<float>();
    }
    bool IsEnd(const std::string& _name) { return event_[_name].timer.GetNow<float>() >= event_[_name].duration; }

    void ImGui();

private:
    const std::string GROUPNAME = "TimeKeeper";
    std::unordered_map<std::string, TimeData> event_;
    GlobalVariables* m_gv_ = nullptr;

};