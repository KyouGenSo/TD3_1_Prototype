#pragma once

#include <string> // std::string
#include <Interfaces/IObserver.h>

class GUI_PauseMenu : public IObserver
{
public:
    GUI_PauseMenu() = default;
    ~GUI_PauseMenu() override = default;


    void    Initialize() override;
    void    Finalize() override {}
    void    Update() override;
    void    OnNotify(const std::string& _name, const std::string& _event) override;


private:
    bool    isDisplay_ = false;


private:
    void ShowPauseMenu();
};