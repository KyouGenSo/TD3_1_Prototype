#include "./Controller.h"
#include <NiGui.h>
#include <Utility/Adaptor.h>

using namespace UI_Widget;

#include <WinApp.h>

void Controller::Initialize(const std::string& _name, const Vector2& _position)
{
    position_ = NiUtil::Adaptor(_position);
    size_statebar_ = { 150.0f, 5.0f };
    size_button_ = { 30.0f, 30.0f };
    InitializeButtonArg(arg_down_, _name + "down", "white.png", { -100.0f, 0.0f });
    InitializeButtonArg(arg_up_, _name + "up", "white.png", { 100.0f, 0.0f });
    statebar_.Initialize();
    statebar_.SetSize(NiUtil::Adaptor(size_statebar_));
    NiVec2 wndsize = { static_cast<float>(WinApp::clientWidth / 2), static_cast<float>(WinApp::clientHeight / 2) };
    statebar_.SetPosition(NiUtil::Adaptor(wndsize - size_statebar_ / 2.0f + position_));
    statebar_.SetEnableTimer(false);
    statebar_.Display(true);
}

void Controller::Update()
{
    isChanged_ = false;
    this->UpdateWindow();
    statebar_.SetCurrentValue(value_);
    statebar_.Update();
}

void Controller::Draw2d()
{
    statebar_.Draw2d();
}

void UI_Widget::Controller::SetValue(float _current)
{
    value_ = _current;
    statebar_.SetCurrentValue(_current);
}

void UI_Widget::Controller::SetMax(float _max)
{
    max_ = _max;
    statebar_.SetMaxValue(_max);
}

void UI_Widget::Controller::InitializeButtonArg(NiGui_Arg_Button& _arg, const std::string& _id, const std::string& _tex, const NiVec2& _pos) const
{
    NiGui_StandardPoint center = NiGui_StandardPoint::Center;
    _arg.id = _id;
    _arg.textureName = _tex;
    _arg.color = NiGui::WHITE;
    _arg.position = _pos + position_;
    _arg.size = size_button_;
    _arg.texSize = size_button_;
    _arg.anchor = center;
    _arg.pivot = center;
}

void UI_Widget::Controller::UpdateWindow()
{
    auto confirm = NiGui_ButtonState::Confirm;

    if (NiGui::Button(arg_up_) == confirm)
    {
        value_ += max_ * 0.1f;
        isChanged_ = true;
    }
    if (NiGui::Button(arg_down_) == confirm)
    {
        value_ -= max_ * 0.1f;
        isChanged_ = true;
    }

    if (value_ > max_) value_ = max_;
    if (value_ < 0) value_ = 0;
}
