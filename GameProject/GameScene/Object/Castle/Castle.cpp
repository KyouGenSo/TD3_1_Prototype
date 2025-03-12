#include "Castle.h"

void Castle::Initialize() {
    Object::Initialize();

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetCamera(pCamera_);
    model_->SetModel("box.gltf");

    transform_ = {
        {10, 10, 10},
        {0, 0, 0},
        {0, 0, -50}
    };

    model_->SetScale(transform_.scale);
    model_->SetTranslate(transform_.translate);
}

void Castle::Update() {

    model_->Update();
}

void Castle::Draw() {
    model_->Draw();
}
