#include "TimeKeeper.h"

#include <imgui.h>

void TimeKeeper::Initialize()
{
    event_.clear();
    m_gv_ = GlobalVariables::GetInstance();
    m_gv_->CreateGroup(GROUPNAME);
}

void TimeKeeper::Update()
{
    for (auto& [key, value] : event_)
    {
        if (value.timer.GetNow<float>() <= 0.0)
        {
            value.timer.Stop();
        }
    }
}

void TimeKeeper::Run(const std::string& _name)
{
    event_[_name].timer.Reset();
    event_[_name].timer.Start();
}

void TimeKeeper::Reset(const std::string& _name)
{
    event_[_name].timer.Reset();
}

void TimeKeeper::AddEvent(const std::string& _name, float _duration)
{
    event_[_name] = { Timer(), _duration };
    m_gv_->AddItem(GROUPNAME, _name, _duration);
}

void TimeKeeper::Load()
{
    m_gv_->LoadFile(GROUPNAME);

    for (auto& [key, value] : event_)
    {
        value.duration = m_gv_->GetValueFloat(GROUPNAME, key);
    }
}

void TimeKeeper::ImGui()
{
    if (ImGui::Begin("TimeKeeper"))
    {
        if (ImGui::Button("Load")) Load();
        for (auto& [key, value] : event_)
        {
            if (ImGui::TreeNode(key.c_str()))
            {
                float now = value.duration - value.timer.GetNow<float>();
                ImGui::Text("%.1f", now > 0.0f ? now : 0.0f);
                ImGui::ProgressBar(value.timer.GetNow<float>() / value.duration, ImVec2(0.0f, 0.0f), "");
                ImGui::Text("Duration : %f", value.duration);
                if (ImGui::Button("Run"))
                {
                    Run(key);
                }
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}
