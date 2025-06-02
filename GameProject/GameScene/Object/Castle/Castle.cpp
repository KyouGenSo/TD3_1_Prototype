#include "Castle.h"

#include "Collision/Collider.h"
#include "Type/ColliderType.h"
#include <Utility/Adaptor.h>


void Castle::Initialize() {
    Object::Initialize();

    name_ = "Castle";

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("castle.gltf");

    // 初期化用 - 外部から設定するためこの値は適用されない
    transform_ = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    model_->SetScale(transform_.scale);
    model_->SetTranslate(transform_.translate);

    statusInit_
        .setAttack(5000)
        .setHp(500)
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(500);
    statusCurrent_ = statusInit_;


    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_
        ->SetEvent(Collision::EventType::Trigger, [this](const Collision::Collider* pObj) { this->OnCollisionTrigger(pObj); })
        ->SetSize(Collision::Vec3{20.f, 20.f, 22.5f})
        ->SetType(Collision::Type::AABB)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        //->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
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
    Object* object = static_cast<Object*>(_pCollider->GetOwner());
    if (_pCollider->GetAttribute() & static_cast<uint32_t>(Collider::Type::ENEMY)){
        Object::StatusUpdateOnCollision(_pCollider);
    }
}

void Castle::ImGui()
{
    Object::ImGui([&]() { statusCurrent_.ImGui("Castle"); });
}
