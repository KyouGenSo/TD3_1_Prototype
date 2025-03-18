#include "GUI_LvUP.h"

#include <NiUI/NiUI.h>

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
    auto center = NiUI_StandardPoint::Center;

    auto confirm = NiUI_ButtonState::Confirm;

    /// レベルアップ画面を表示する処理
    bool isClose = false;

    if (NiUI::BeginDiv("LvUP", TEXTUREPATH_, NiUI::BLACK, { 0, 0 }, { 800, 450 }, center, center))
    {
        if (NiUI::Button("Close1", TEXTUREPATH_, { -250, 0 }, { 200, 350 }, center, center) == confirm)
        {
            isClose = true;
        }
        if (NiUI::Button("Close2", TEXTUREPATH_, { 0, 0 }, { 200, 350 }, center, center) == confirm)
        {
            isClose = true;
        }
        if (NiUI::Button("Close3", TEXTUREPATH_, { 250, 0 }, { 200, 350 }, center, center) == confirm)
        {
            isClose = true;
        }
        NiUI::EndDiv();
    }


    if (isClose)
    {
        isDisplay_ = false;
    }

    return;
}
