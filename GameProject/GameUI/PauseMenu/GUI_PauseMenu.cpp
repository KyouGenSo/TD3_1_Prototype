#include "GUI_PauseMenu.h"

#include <NiUI/NiUI.h>

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
    if (_event == "open_pause_menu")
    {
        showPauseMenu_ = true;
    }
    else if (_event == "close_pause_menu")
    {
        showPauseMenu_ = false;
    }
    else if (_event == "toggle_pause_menu")
    {
        showPauseMenu_ = !showPauseMenu_;
    }
}

void GUI_PauseMenu::ShowPauseMenu()
{
    auto center = NiUI_StandardPoint::Center;
    auto confirm = NiUI_ButtonState::Confirm;

    if (NiUI::BeginDiv("PauseMenu", "white.png", NiUI::BLACK, {}, { 250, 250 }, center, center))
    {
        if (NiUI::Button("Resume", "white.png", { 0.0f, -50.0f }, { 150.0f, 50.0f }, center, center) == confirm)
        {
            showPauseMenu_ = false;
        }
        if (NiUI::Button("Exit", "white.png", { 0.0f, 50.0f }, { 150.0f, 50.0f }, center, center) == confirm)
        {
            showPauseMenu_ = false;
        }
    }
    NiUI::EndDiv();
}
