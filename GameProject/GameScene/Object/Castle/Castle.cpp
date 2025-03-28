#include "Castle.h"

#include "GameScene/Object/Collision/Collider.h"

void Castle::Initialize() {
    Object::Initialize();

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetCamera(pCamera_);
    model_->SetModel("box.gltf");

    // 初期化用 - 外部から設定するためこの値は適用されない
    transform_ = {
        {10, 10, 10},
        {0, 0, 0},
        {0, 0, -50}
    };

    model_->SetScale(transform_.scale);
    model_->SetTranslate(transform_.translate);

    collider_ = std::make_unique<Collider>();
    collider_->SetOwner(this)
        ->SetSize(Vector3{.x= 5.f, .y= 5.f, .z= 5.f})
        ->SetEvent([&](const auto& c){})
        ->SetType(Collider::Type::PLAYER)
        ->SetIgnore(Collider::Type::STAGE)
        ->SetIgnore(Collider::Type::PLAYER);
}

void Castle::Update() {

    model_->SetScale(transform_.scale);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void Castle::Draw() {
    model_->Draw();
}
