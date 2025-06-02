#include "wnd_pausemenu.h"

#include <GameUI/ColorResolver/ColorResolver.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>
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
    if (isDisplay_)
    {
        // PauseMenuの描画処理
        ShowPauseMenu();
    }
}

void GUI_PauseMenu::OnNotify(const std::string& _name, const std::string& _event)
{
    if (_name != "pause_menu") return;

    auto dtm = DeltaTimeManager::GetInstance();
    if (_event == "open")
    {
        dtm->SetDeltaTime(1, 0.0f);
        notifier_->Notify("OnWindowOpen", true);
        isDisplay_ = true;
    }
    else if (_event == "close")
    {
        dtm->SetDeltaTime(1, 1.0f / 60.0f);
        notifier_->Notify("RequestOpenSetting", true);
        if (isDisplay_) notifier_->Notify("OnWindowOpen", true);
        isDisplay_ = false;
    }
    else if (_event == "toggle")
    {
        if (isDisplay_) dtm->SetDeltaTime(1, 1.0f / 60.0f);
        else dtm->SetDeltaTime(1, 0.0f);
        isDisplay_ = !isDisplay_;
        notifier_->Notify("OnWindowOpen", isDisplay_);
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
        if (NiGui::Button("Resume", "resume.png", NiGui::WHITE, { 0.0f, -70.0f }, { 150.0f, 50.0f }, {}, center, center) == confirm)
        {
            dtm->SetDeltaTime(1, 1.0f / 60.0f);
            notifier_->Notify("OnWindowOpen", false);
            isDisplay_ = false;
        }
        if (NiGui::Button("Option", "resume.png", NiGui::WHITE, { 0.0f, 0.0f }, { 150.0f, 50.0f }, {}, center, center) == confirm)
        {
            dtm->SetDeltaTime(1, 1.0f / 60.0f);
            notifier_->Notify("RequestOpenSetting", true);
            isDisplay_ = false;
        }
        if (NiGui::Button("Exit", "exit.png", NiGui::WHITE, { 0.0f, 70.0f }, { 150.0f, 50.0f }, {}, center, center) == confirm)
        {
            dtm->SetDeltaTime(1, 1.0f / 60.0f);
            GameEventNotifier::GetInstance()->Notify("Exit", nullptr);
            isDisplay_ = false;
        }
    }
    NiGui::EndDiv();
}
