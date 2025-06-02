#pragma once

#include <Interfaces/IObserver.h>
#include <GameUI/Controller/Controller.h>

class Wnd_Setting : public IObserver
{
public:
    // Common functions
    void    Initialize() override;              // Initialize widgets

    void    OnNotify(
        const std::string& _name, 
        const std::string& _event) override;    // For callback

    void    Update() override;                  // Update and show window
    void    Draw2d();
    void    Finalize() override;                // Unregister callback funcs

private:
    void    Show();

    bool    isDisplay_  = false;

    // Callback IDs
    uint32_t        id_callback_change_display_ = 0u;

    // Widgets
    std::unique_ptr<Sprite> sprite_background_ = nullptr;
    NiGui_Arg_Div           arg_window_             = {};
    UI_Widget::Controller   controller_volume_      = {};
    UI_Widget::Controller   controller_sens_        = {};

    // state
    float curret_volume_ = 100.0f;
    float current_sens_ = 50.0f;
    
};