#include "GUI_PauseMenu.h"

#include <GameUI/ColorResolver/ColorResolver.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <string>
#include <Math/NiVec4.h>
#include <Type/NiGui_Enum.h>
#include <Utility/Adaptor.h>
#include <GameUI/ColorName/ColorName.h>
#include <NiGui.h>

void GUI_PauseMenu::Initialize()
{
}

void GUI_PauseMenu::Update()
{
    if (showPauseMenu_)
    {
        // PauseMenuの描画処理
        ShowPauseMenu();
    }
}

void GUI_PauseMenu::OnNotify(const std::string& _event)
{
    auto dtm = DeltaTimeManager::GetInstance();
    if (_event == "open_pause_menu")
    {
        dtm->SetDeltaTime(1, 0.0f);
        showPauseMenu_ = true;
    }
    else if (_event == "close_pause_menu")
    {
        dtm->SetDeltaTime(1, 1.0f / 60.0f);
        showPauseMenu_ = false;
    }
    else if (_event == "toggle_pause_menu")
    {
        if (showPauseMenu_) dtm->SetDeltaTime(1, 1.0f / 60.0f);
        else dtm->SetDeltaTime(1, 0.0f);
        showPauseMenu_ = !showPauseMenu_;
    }
}

void GUI_PauseMenu::ShowPauseMenu()
{
    auto center = NiGui_StandardPoint::Center;
    auto confirm = NiGui_ButtonState::Confirm;

    NiVec4 bgcolor = NiUtil::Adaptor(ColorResolver::GetInstance()->Resolve(ColorName::Background).toVector4());
    NiGui::BeginDiv("PauseMenu_Background", "white.png", bgcolor, {}, { 1600,900 }, center, center);
    NiGui::EndDiv();

    auto dtm = DeltaTimeManager::GetInstance();

    if (NiGui::BeginDiv("PauseMenu", "white.png", NiGui::BLACK, {}, { 250, 250 }, center, center))
    {
        if (NiGui::Button("Resume", "white.png", NiGui::WHITE, { 0.0f, -50.0f }, { 150.0f, 50.0f }, {}, center, center) == confirm)
        {
            dtm->SetDeltaTime(1, 1.0f / 60.0f);
            showPauseMenu_ = false;
        }
        if (NiGui::Button("Exit", "white.png", NiGui::WHITE, { 0.0f, 50.0f }, { 150.0f, 50.0f }, {}, center, center) == confirm)
        {
            dtm->SetDeltaTime(1, 1.0f / 60.0f);
            showPauseMenu_ = false;
        }
    }
    NiGui::EndDiv();
}
