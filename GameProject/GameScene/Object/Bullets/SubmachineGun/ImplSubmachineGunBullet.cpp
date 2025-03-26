#include "MachineGunBullet.h"

MachineGunBullet::Bullet* MachineGunBullet::Bullet::Initialize() {
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("box.gltf");
    model_->SetScale({.2f, .2f, .2f});

    collider_ = std::make_unique<Collider>();
    collider_->SetEvent([&](auto c){OnCollisionTrigger(c); })
        ->SetSize(0.1f)
        ->SetType(Collider::Type::ALLY)
        ->SetIgnore(Collider::Type::ALLY)
        ->SetIgnore(Collider::Type::STAGE);
    Update();
    return this;
}

void MachineGunBullet::Bullet::Update() {
    transform_.translate += forward_ * speed_;

    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void MachineGunBullet::Bullet::Draw() {
    model_->Draw();
}

MachineGunBullet::Bullet* MachineGunBullet::Bullet::SetOriginalPosition(const Vector3& _pos)
{
    origin = _pos;
    transform_.translate = origin;
    return this;
}

MachineGunBullet::Bullet* MachineGunBullet::Bullet::SetRotate(const Vector3& _rotate) {
    transform_.rotate = _rotate;
    return this;
}

MachineGunBullet::Bullet* MachineGunBullet::Bullet::SetForward(const Vector3& _forward) {
    forward_ = _forward;
    return this;
}

MachineGunBullet::Bullet* MachineGunBullet::Bullet::SetSpeed(float _speed) {
    speed_ = _speed;
    return this;
}

void MachineGunBullet::Bullet::OnCollisionTrigger(const Collider* _other) {
    dead = true;
}
