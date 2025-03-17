#include "GUI_LvUP.h"

#include <NiUI/NiUI.h>
#include <cassert>

#include <Audio.h>

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

    NiUI::DragItemArea("DragItemArea1", TEXTUREPATH_, NiUI::BLUE, { -240, 150 }, { 120, 120 }, center, center);
    NiUI::DragItemArea("DragItemArea2", TEXTUREPATH_, NiUI::BLUE, { 0, 150 }, { 120, 120 }, center, center);
    NiUI::DragItemArea("DragItemArea3", TEXTUREPATH_, NiUI::BLUE, { 240, 150 }, { 120, 120 }, center, center);
    NiUI::DragItem("DragItem1", TEXTUREPATH_, NiUI::YELLOW, { -240, -150 }, { 100, 100 }, center, center);
    NiUI::DragItem("DragItem2", TEXTUREPATH_, NiUI::CIAN, { 0, -150 }, { 100, 100 }, center, center);
    NiUI::DragItem("DragItem3", TEXTUREPATH_, NiUI::MAGENTA, { 240, -150 }, { 100, 100 }, center, center);


    if (isClose)
    {
        isDisplay_ = false;
    }

    return;
}
