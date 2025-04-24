#include "MachineGunBullet.h"
#include "Type/ColliderType.h"

MachineGunBullet::Bullet* MachineGunBullet::Bullet::Initialize() {
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("box.gltf");
    model_->SetScale({.2f, .2f, .2f});

    collider_ = std::make_unique<Collision::Collider>();
    collider_->SetEvent(Collision::EventType::Trigger, [&](auto c){OnCollisionTrigger(c); })
        ->SetSize(0.2f)
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetOwner(this)
        ->Enable();
    Update();
    return this;
}

void MachineGunBullet::Bullet::Update() {
    if (dead)return;
    transform_.translate += forward_ * speed_;

    if (50.f <= (transform_.translate - origin).Length()){
        dead = true;
    }

    collider_->SetTranslate(Adaptor(transform_.translate));

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

void MachineGunBullet::Bullet::OnCollisionTrigger(const Collision::Collider* _other) {
    dead = true;
}
