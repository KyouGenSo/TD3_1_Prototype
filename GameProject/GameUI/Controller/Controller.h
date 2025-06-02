#pragma once

#include <Type/NiGui_Type_Argument.h>
#include <GameUI/HPBar/HPBar.h>
#include <Math/NiVec2.h>
#include <string>
#include <Vector2.h>

namespace UI_Widget
{
    class Controller
    {
    public:
        void    Initialize(const std::string& _name, const Vector2& _position);
        void    Update();
        void    Draw2d();

        // Setter
        void    SetValue(float _current);
        void    SetMax(float _max);

        // Getter
        float   GetValue() const { return value_; };

    private:
        void    InitializeButtonArg(NiGui_Arg_Button& _arg, const std::string& _id, const std::string& _tex, const NiVec2& _pos) const;
        void    UpdateWindow();

        std::string         name_           = {};
        float               scale_          = 1.0f;
        NiVec2              size_button_    = {};
        NiGui_Arg_Button    arg_down_       = {};
        NiGui_Arg_Button    arg_up_         = {};
        HPBar               statebar_       = {};
        NiVec2              size_statebar_  = {};
        float               value_          = {};
        float               max_            = {};
        NiVec2              position_       = {};
    };
}