#include "AssaultBullet.h"

#include <ModelManager.h>

#include "QuatFunc.h"

void AssaultBullet::Bullet::Initialize()
{
    ModelManager::GetInstance()->LoadModel("box.gltf");

    pLifeTimer_ = std::make_unique<Timer>();
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("box.gltf");
    model_->SetScale(Vector3(0.3f, 0.3f, 0.3f));

    collider_ = std::make_unique<Collider>();
    collider_->SetEvent([this](const auto& c){this->OnCollisionTrigger(c); })
        ->SetSize(0.3f)
        ->SetType(Collider::Type::P_BULLET)
        ->SetIgnore(Collider::Type::PLAYER)
        ->SetIgnore(Collider::Type::P_BULLET)
        ->SetIgnore(Collider::Type::STAGE);
    Update();
}

void AssaultBullet::Bullet::Update()
{
    transform_.translate += forward_ * speed_;

    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void AssaultBullet::Bullet::Draw()
{
    if (!model_)return;
    model_->Draw();
}

void AssaultBullet::Bullet::OnCollisionTrigger(const Collider* _other) {
    if (_other->GetType() == Collider::Type::ENEMY){
        hit = true;
    }
}

bool AssaultBullet::Bullet::IsHit() {
    return hit;
}
