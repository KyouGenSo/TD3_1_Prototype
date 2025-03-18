#include "GUI_LvUP.h"

#include <NiGui.h>

#include <imgui.h>

void GUI_LvUP::OnNotify(const std::string& _event)
{
    if (_event == "open_lvup")
    {
        isDisplay_ = true;
    }
    else if (_event == "close_lvup")
    {
        isDisplay_ = false;
    }
    else if (_event == "toggle_lvup")
    {
        isDisplay_ = !isDisplay_;
    }
}

void GUI_LvUP::Update()
{
    /// 通知されたら表示する
    if (isDisplay_)
    {
        ShowLvUP();
    }
}

void GUI_LvUP::ImGui()
{

}

void GUI_LvUP::ShowLvUP()
{
    auto center = NiGui_StandardPoint::Center;

    auto confirm = NiGui_ButtonState::Confirm;

    /// レベルアップ画面を表示する処理
    bool isClose = false;

    if (NiGui::BeginDiv("LvUP", TEXTUREPATH_, NiGui::BLACK, { 0, 0 }, { 800, 450 }, center, center))
    {
        if (NiGui::Button("Close1", TEXTUREPATH_, NiGui::WHITE, { -250, 0 }, { 200, 350 }, center, center) == confirm)
        {
            isClose = true;
        }
        if (NiGui::Button("Close2", TEXTUREPATH_, NiGui::WHITE, { 0, 0 }, { 200, 350 }, center, center) == confirm)
        {
            isClose = true;
        }
        if (NiGui::Button("Close3", TEXTUREPATH_, NiGui::WHITE, { 250, 0 }, { 200, 350 }, center, center) == confirm)
        {
            isClose = true;
        }
        NiGui::EndDiv();
    }


    if (isClose)
    {
        isDisplay_ = false;
    }

    return;
}
