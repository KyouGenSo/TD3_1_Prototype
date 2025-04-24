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

    if (isSelected_)
    {
        gameController_->HandleConfirmCard(selectedCard_);
        isSelected_ = false;
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

    if (NiGui::BeginDiv("LvUP", TEXTUREPATH_, NiGui::WHITE, { 0, 0 }, { 1100, 600 }, center, center))
    {
        if (NiGui::Button("Attack", TEX_CARD_ATTACK_, NiGui::WHITE, { -330, 50 }, { 200, 350 }, { 273, 423 }, center, center) == confirm)
        {
            selectedCard_ = "Strong";
            isClose = true;
        }
        if (NiGui::Button("Defence", TEX_CARD_DEFENCE_, NiGui::WHITE, { 0, 50 }, { 200, 350 }, { 273, 423 }, center, center) == confirm)
        {
            selectedCard_ = "Tough";
            isClose = true;
        }
        if (NiGui::Button("Steel", TEX_CARD_STEEL_, NiGui::WHITE, { 330, 50 }, { 200, 350 }, { 273, 423 }, center, center) == confirm)
        {
            selectedCard_ = "Steel";
            isClose = true;
        }
        NiGui::EndDiv();
    }


    if (isClose)
    {
        isDisplay_ = false;
        isSelected_ = true;
    }

    return;
}
