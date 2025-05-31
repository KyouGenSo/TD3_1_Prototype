#include "Castle.h"

#include "Collision/Collider.h"
#include "Type/ColliderType.h"
#include <Utility/Adaptor.h>

void Castle::Initialize() {
    Object::Initialize();

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("box.gltf");

    // 初期化用 - 外部から設定するためこの値は適用されない
    transform_ = {
        {1, 1, 1},
        {0, 0, 0},
        {0, 0, -50}
    };

    model_->SetScale(transform_.scale);
    model_->SetTranslate(transform_.translate);

    statusInit_
        .setAttack(5000)
        .setHp(5000)
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(5000);
    statusCurrent_ = statusInit_;


    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_
        ->SetEvent(Collision::EventType::Trigger, [this](const Collision::Collider* pObj) { this->OnCollisionTrigger(pObj); })
        ->SetSize(Collision::Vec3{10.f, 10.f, 10.f})
        ->SetType(Collision::Type::AABB)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->SetOwner(this)
        ->Enable();

    #ifdef _DEBUG
    enableUpdateStatus_ = false;
    #endif // DEBUG
}

void Castle::Update() {

    model_->SetScale(transform_.scale);
    model_->SetTranslate(transform_.translate);
    model_->Update();
    pCollider_->SetTranslate(Adaptor(transform_.translate));
}

void Castle::Draw() {
    model_->Draw();
}

void Castle::OnCollisionTrigger(const Collision::Collider* _pCollider)
{
    Object::StatusUpdateOnCollision(_pCollider);
}

void Castle::ImGui()
{
    Object::ImGui([&]() { statusCurrent_.ImGui("Castle"); });
}
