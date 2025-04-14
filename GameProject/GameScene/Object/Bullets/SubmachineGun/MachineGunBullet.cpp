#include "MachineGunBullet.h"

#include <numbers>
#include <Collision/Collider.h>

#include "Type/ColliderType.h"

void MachineGunBullet::Initialize() {
    BulletBase::Initialize();

    type_ = WeaponType::MachineGun;
    speed_ = 0.9f;

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("AnimatedCube.gltf");
    model_->SetScale({0.2f, 0.2f, 0.2f});

    CalcLifeTime();

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_
        ->SetEvent(Collision::EventType::Trigger, [&](const Collision::Collider* pCol){ OnCollisionTrigger(pCol); })
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetSize(0.2f)
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE));
}

void MachineGunBullet::Update() {
    if (isChainBullet_){
        UpdateChain();
    } else{
        UpdateNormal();
    }
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
    if (pNext_){
        pNext_->Update();
    }
    isDead_ = CheckLifeTime();
    isDead_ = isDead_ ? true : 20.f <= (transform_.translate - origin).Length();
}

void MachineGunBullet::Draw() {
    if (isDead_ && !pNext_)return;

    if(isChainBullet_){
        for(const auto& bullet : bullets_){
            bullet->Draw();
        }
    } else{
        model_->Draw();
    }
    
    if (pNext_){
        pNext_->Draw();
    }
}

void MachineGunBullet::OnCollisionTrigger(const Collision::Collider* _collider) {
    if(isDead_ || pCollider_->IsDisabled())return;

    isDead_ = true;
    pCollider_->Disable();

    Next();
}

void MachineGunBullet::InitializeNormal() {
    origin = transform_.translate;
}

void MachineGunBullet::InitializeChain() {
    Vector3 pos = transform_.translate;
    pos.y = min(0.5f, pos.y);
    bullets_.resize(32);
    float angle = 0.0f;
    for (size_t i = 0; i < bullets_.size(); i++){
        angle = std::numbers::pi_v<float> / static_cast<float>(bullets_.size()) * i * 2.0f;
        bullets_[i] = std::make_unique<Bullet>();
        bullets_[i]->Initialize()
            ->SetOriginalPosition(pos)
            ->SetRotate({0.0f, angle, 0.0f})
            ->SetForward({sin(angle), 0.0f, cos(angle)})
            ->SetSpeed(speed_);
    }
}

void MachineGunBullet::UpdateNormal() {
    transform_.translate += forward_ * speed_;

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void MachineGunBullet::UpdateChain() {
    std::erase_if(bullets_, [&](const auto& b) {return b->IsDead();});
    for (const auto& bullet : bullets_){
        bullet->Update();
    }
}
