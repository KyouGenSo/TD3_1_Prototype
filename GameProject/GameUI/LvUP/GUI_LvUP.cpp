#include "GUI_LvUP.h"

#include <GameSystem/Reinforcement/Manager/ReinforcementManager.h>

#include <NiGui.h>

#include <imgui.h>
#include <WinApp.h>
#include <Utility/Adaptor.h>

void GUI_LvUP::Initialize()
{
    colorResolver_ = ColorResolver::GetInstance();

    arg_div_background_.id = "Background_LvUP";
    arg_div_background_.textureName = "white.png";
    arg_div_background_.color = NiUtil::Adaptor(colorResolver_->Resolve(ColorName::Background).toVector4());
    arg_div_background_.position = { 0, 0 };
    arg_div_background_.size = { WinApp::clientWidth, WinApp::clientHeight };
    arg_div_background_.anchor = NiGui_StandardPoint::Center;
    arg_div_background_.pivot = NiGui_StandardPoint::Center;


    arg_div_window_.id = "LvUP";
    arg_div_window_.textureName = TEXTUREPATH_;
    arg_div_window_.color = NiGui::WHITE;
    arg_div_window_.position = { 0, 0 };
    arg_div_window_.size = { 1100, 600 };
    arg_div_window_.anchor = NiGui_StandardPoint::Center;
    arg_div_window_.pivot = NiGui_StandardPoint::Center;

    arg_button_left_.id = "Button_LvUP_Left(not updated)";
    arg_button_left_.textureName = "Button_LvUP_Left(not updated)";
    arg_button_left_.color = NiGui::WHITE;
    arg_button_left_.position = { -330, 50 };
    arg_button_left_.size = { 200, 350 };
    arg_button_left_.texSize = { 273, 423 };
    arg_button_left_.anchor = NiGui_StandardPoint::Center;
    arg_button_left_.pivot = NiGui_StandardPoint::Center;

    arg_button_center_.id = "Button_LvUP_Center(not updated)";
    arg_button_center_.textureName = "Button_LvUP_Center(not updated)";
    arg_button_center_.color = NiGui::WHITE;
    arg_button_center_.position = { 0, 50 };
    arg_button_center_.size = { 200, 350 };
    arg_button_center_.texSize = { 273, 423 };
    arg_button_center_.anchor = NiGui_StandardPoint::Center;
    arg_button_center_.pivot = NiGui_StandardPoint::Center;

    arg_button_right_.id = "Button_LvUP_Right(not updated)";
    arg_button_right_.textureName = "Button_LvUP_Right(not updated)";
    arg_button_right_.color = NiGui::WHITE;
    arg_button_right_.position = { 330, 50 };
    arg_button_right_.size = { 200, 350 };
    arg_button_right_.texSize = { 273, 423 };
    arg_button_right_.anchor = NiGui_StandardPoint::Center;
    arg_button_right_.pivot = NiGui_StandardPoint::Center;
}

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

void GUI_LvUP::OnResize(Vector2 _size)
{
    arg_div_background_.size = NiUtil::Adaptor(_size);
}

void GUI_LvUP::Update()
{
    /// 通知されたら表示する
    if (isDisplay_)
    {
        if (!isPickRandom_) PickCardName();

        ShowLvUP();
    }

    if (isSelected_)
    {
        gameController_->HandleConfirmCard(selectedCard_);
        isSelected_ = false;
        isPickRandom_ = false;
    }
}

void GUI_LvUP::ImGui()
{

}

void GUI_LvUP::ShowLvUP()
{
    auto confirm = NiGui_ButtonState::Confirm;

    arg_button_left_.id = rfCards_[0].name;
    arg_button_left_.textureName = rfCards_[0].imagepath;
    
    arg_button_center_.id = rfCards_[1].name;
    arg_button_center_.textureName = rfCards_[1].imagepath;

    arg_button_right_.id = rfCards_[2].name;
    arg_button_right_.textureName = rfCards_[2].imagepath;

    /// レベルアップ画面を表示する処理
    bool isClose = false;

    if (NiGui::BeginDiv(arg_div_background_))
    {
        if (NiGui::BeginDiv(arg_div_window_))
        {
            if (NiGui::Button(arg_button_left_) == confirm)
            {
                selectedCard_ = rfCards_[0].name;
                isClose = true;
            }
            if (NiGui::Button(arg_button_center_) == confirm)
            {
                selectedCard_ = rfCards_[1].name;
                isClose = true;
            }
            if (NiGui::Button(arg_button_right_) == confirm)
            {
                selectedCard_ = rfCards_[2].name;
                isClose = true;
            }
            NiGui::EndDiv();
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

void GUI_LvUP::PickCardName()
{
    // クリア
    rfCards_.clear();

    auto rm = ReinforcementManager::GetInstance();

    int picked = 0;
    while (picked < 3)
    {
        // 選ぶ
        auto pickedCard = rm->GetRandomCard(rm->kFilename_json_status_);

        bool isFoundSame = false;
        for (auto& card : rfCards_)
        {
            if (card.name == pickedCard.name)
            {
                isFoundSame = true;
                break;
            }
        }

        if (isFoundSame) continue;

        rfCards_.push_back(pickedCard);
        ++picked;
    }

    isPickRandom_ = true;
}
