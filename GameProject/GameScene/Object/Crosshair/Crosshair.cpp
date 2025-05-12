#include "Crosshair.h"

void Crosshair::Initialize() {
    Object::Initialize();

    sprite_ = std::make_unique<Sprite>();
    sprite_->Initialize("circle.png");

    sprite_->SetAnchorPoint({.x= 0.5f, .y= 0.5f});
    sprite_->SetPos({.x = static_cast<float>(WinApp::clientWidth) / 2.f, .y = static_cast<float>(WinApp::clientHeight) / 2.f});

    sprite_->SetSize({.x= 32.f, .y= 32.f});
}

void Crosshair::Update() {
    Object::Update();

    sprite_->Update();
}

void Crosshair::Draw() {
    sprite_->Draw();
}
