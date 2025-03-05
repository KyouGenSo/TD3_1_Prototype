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
    auto leftCenter = NiUI_StandardPoint::LeftCenter;

    /// レベルアップ画面を表示する処理
    bool isClose = false;


    if (NiUI::BeginDiv("TestDiv", { 0.0f, 0.0f }, { 230.0f, 120.0f }, center, center))
    {
        if (NiUI::Button("Test1", TEXTUREPATH_, { 10.0f, 0.0f }, { 100.0f, 100.0f }, leftCenter, leftCenter) == NiUI_ButtonState::Confirm)
        {
            isClose = true;
        }
        if (NiUI::Button("Test2", TEXTUREPATH_, { 120.0f, -25.0f }, { 100.0f, 30.0f }, leftCenter, leftCenter) == NiUI_ButtonState::Confirm)
        {
            isClose = true;
        }
        if (NiUI::Button("Test3", TEXTUREPATH_, { 120.0f, 25.0f }, { 100.0f, 30.0f }, leftCenter, leftCenter) == NiUI_ButtonState::Confirm)
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
