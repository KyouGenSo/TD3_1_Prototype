#include "AssaultBullet.h"

#include <ModelManager.h>

#include "QuatFunc.h"
#include "Type/ColliderType.h"

void AssaultBullet::Bullet::Initialize()
{
    ModelManager::GetInstance()->LoadModel("box.gltf");

    pLifeTimer_ = std::make_unique<Timer>();
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("box.gltf");
    model_->SetScale(Vector3(0.3f, 0.3f, 0.3f));

    collider_ = std::make_unique<Collision::Collider>();
    collider_->SetEvent(Collision::EventType::Trigger, [this](const auto& c){this->OnCollisionTrigger(c); })
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetSize(0.3f)
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE));
    Update();
}

void AssaultBullet::Bullet::Update()
{
    transform_.translate += forward_ * speed_;

    collider_->SetTranslate(Adaptor(transform_.translate));

    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void AssaultBullet::Bullet::Draw()
{
    if (!model_)return;
    model_->Draw();
}

void AssaultBullet::Bullet::OnCollisionTrigger(const Collision::Collider* _other) {
    if (_other->GetAttribute() & static_cast<uint32_t>(Collider::Type::ENEMY)){
        hit = true;
    }
}

bool AssaultBullet::Bullet::IsHit() {
    return hit;
}
