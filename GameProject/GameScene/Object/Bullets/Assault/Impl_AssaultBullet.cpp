#include "AssaultBullet.h"

#include <ModelManager.h>

void AssaultBullet::Bullet::Initialize()
{
    ModelManager::GetInstance()->LoadModel("box.gltf");

    pLifeTimer_ = std::make_unique<Timer>();
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("box.gltf");
    model_->SetScale(Vector3(0.3f, 0.3f, 0.3f));
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
