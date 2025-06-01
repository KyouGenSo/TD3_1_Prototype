#pragma once

#include <string> // std::string
#include <Interfaces/IObserver.h>

class GUI_PauseMenu : public IObserver
{
public:
    GUI_PauseMenu() = default;
    ~GUI_PauseMenu() override = default;

    void Initialize();
    void Update();
    void OnNotify(const std::string& _event) override;

private:
    bool isDisplay_ = false;

private:
    void ShowPauseMenu();
};