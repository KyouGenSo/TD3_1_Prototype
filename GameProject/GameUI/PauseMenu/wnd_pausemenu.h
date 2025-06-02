#pragma once

#include <string> // std::string
#include <Interfaces/IObserver.h>
#include <Vector2.h>

class GUI_PauseMenu : public IObserver
{
public:
    GUI_PauseMenu() = default;
    ~GUI_PauseMenu() override = default;


    void    Initialize() override;
    void    Finalize() override {}
    void    Update() override;
    void    OnNotify(const std::string& _name, const std::string& _event) override;
    void    OnResize(Vector2 size);


private:
    Vector2 displaySize_    = { 1600.0f, 900.0f };


private:
    void ShowPauseMenu();
};