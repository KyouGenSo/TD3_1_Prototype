#include "Castle.h"

#include "Collision/Collider.h"

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

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_
        ->SetEvent(Collision::EventType::Trigger, [&](const auto& c){})
        ->SetSize(Collision::Vec3{5.f, 5.f, 5.f})
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->Enable();
}

void Castle::Update() {

    model_->SetScale(transform_.scale);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void Castle::Draw() {
    model_->Draw();
}
