#include "GUI_LvUP.h"

#include <NiUI/NiUI.h>
#include <cassert>

#include <Audio.h>

void GUI_LvUP::OnNotify(const std::string& _event)
{
    if (_event == "lvup")
    {
        isDisplay_ = true;
    }
    else
    {
        assert(false && "Invalid event");
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

void GUI_LvUP::ShowLvUP()
{
    auto center = NiUI_StandardPoint::Center;

    /// レベルアップ画面を表示する処理
    bool isClose = false;

    if (NiUI::Button("Card1", TEXTUREPATH_, { -200.0f, 50.0f }, { 240.0f, 360.0f }, center, center) == NiUI_ButtonState::Confirm)
    {
        isClose = true;
    }
    if (NiUI::Button("Card2", TEXTUREPATH_, { 0.0f, 0.0f }, { 240.0f, 360.0f }, center, center) == NiUI_ButtonState::Confirm)
    {
        isClose = true;
    }
    if (NiUI::Button("Card3", TEXTUREPATH_, { 260.0f, 0.0f }, { 240.0f, 360.0f }, center, center) == NiUI_ButtonState::Confirm)
    {
        isClose = true;
    }

    if (isClose)
    {
        isDisplay_ = false;
    }

    return;
}
