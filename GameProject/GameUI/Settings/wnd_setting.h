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

    // Callback IDs
    uint32_t        id_callback_change_display_ = 0u;

    // Window properties
    NiVec2                  wndsize_                = {};

    // Widgets
    std::unique_ptr<Sprite> sprite_background_      = nullptr;
    std::unique_ptr<Sprite> sprite_text_volume_     = nullptr;
    std::unique_ptr<Sprite> sprite_text_sens_       = nullptr;
    UI_Widget::Controller   controller_volume_      = {};
    UI_Widget::Controller   controller_sens_        = {};

    // Widgets properties
    NiVec2                  pos_sprite_volume_      = {};
    NiVec2                  pos_sprite_sens_        = {};
    NiGui_Arg_Div           arg_window_             = {};
    NiVec2                  pos_background_         = {};
    NiVec2                  size_background_        = {};
    // state
    float curret_volume_ = 100.0f;
    float current_sens_ = 50.0f;
    
};