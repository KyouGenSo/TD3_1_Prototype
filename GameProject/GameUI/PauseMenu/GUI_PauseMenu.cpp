#include "GUI_PauseMenu.h"

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
    auto center = NiGui_StandardPoint::Center;
    auto confirm = NiGui_ButtonState::Confirm;


    if (NiGui::BeginDiv("PauseMenu", "white.png", NiGui::BLACK, {}, { 250, 250 }, center, center))
    {
        if (NiGui::Button("Resume", "resume.png", NiGui::WHITE, { 0.0f, -50.0f }, { 150.0f, 50.0f }, {}, center, center) == confirm)
        {
            showPauseMenu_ = false;
        }
        if (NiGui::Button("Exit", "exit.png", NiGui::WHITE, { 0.0f, 50.0f }, { 150.0f, 50.0f }, {}, center, center) == confirm)
        {
            showPauseMenu_ = false;
        }
    }
    NiGui::EndDiv();
}
