#include "GUI_LvUP.h"

#include <GameSystem/Reinforcement/Manager/ReinforcementManager.h>

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
    auto center = NiGui_StandardPoint::Center;

    auto confirm = NiGui_ButtonState::Confirm;

    /// レベルアップ画面を表示する処理
    bool isClose = false;

    if (NiGui::BeginDiv("LvUP", TEXTUREPATH_, NiGui::WHITE, { 0, 0 }, { 1100, 600 }, center, center))
    {
        if (NiGui::Button(rfCards_[0].name, rfCards_[0].imagepath, NiGui::WHITE, {-330, 50}, {200, 350}, {273, 423}, center, center) == confirm)
        {
            selectedCard_ = rfCards_[0].name;
            isClose = true;
        }
        if (NiGui::Button(rfCards_[1].name, rfCards_[1].imagepath, NiGui::WHITE, { 0, 50 }, { 200, 350 }, { 273, 423 }, center, center) == confirm)
        {
            selectedCard_ = rfCards_[1].name;
            isClose = true;
        }
        if (NiGui::Button(rfCards_[2].name, rfCards_[2].imagepath, NiGui::WHITE, { 330, 50 }, { 200, 350 }, { 273, 423 }, center, center) == confirm)
        {
            selectedCard_ = rfCards_[2].name;
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
