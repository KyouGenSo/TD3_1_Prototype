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
    collider_->SetEvent([&](auto c){OnCollisionTrigger(c); })
        ->SetSize(0.1f)
        ->SetType(Collider::Type::ALLY)
        ->SetIgnore(Collider::Type::ALLY)
        ->SetIgnore(Collider::Type::STAGE);
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
    model_->Draw();
}

void AssaultBullet::Bullet::OnCollisionTrigger(const Collider* _other) {
    (void)_other;
}
