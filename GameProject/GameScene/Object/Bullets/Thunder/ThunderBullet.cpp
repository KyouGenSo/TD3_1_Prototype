#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "ThunderBullet.hpp"
#include "Type/ColliderType.h"
#include "Utility/Adaptor.h"

void ThunderBullet::Bullet::Draw() {
}

void ThunderBullet::Bullet::Initialize() {
    BulletBase::Initialize();
    type_ = WeaponType::Thunder;
    speed_ = 6.f;
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("thunderBullet.gltf");
    model_->SetScale({0.2f, 0.2f, 0.2f});
    CalcLifeTime();
    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_
        ->SetEvent(Collision::EventType::Trigger, [&](const Collision::Collider* pCol){ OnCollisionTrigger(pCol); })
        ->SetSize(0.5f)
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetOwner(this)
        ->Enable();

    if (isChainBullet_){
        InitializeChain();
    } else{
        InitializeNormal();
    }
}

void ThunderBullet::Bullet::Update() {
    BulletBase::Update();
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
}

void ThunderBullet::Bullet::OnCollisionTrigger(const Collision::Collider* _collider) {
    if (isDead_ || pCollider_->IsDisabled()) return;
    if (_collider->GetAttribute() & static_cast<uint32_t>(Collider::Type::ENEMY)){
        isDead_ = true;
        pCollider_->Disable();
        NotifyReinforcementManager(_collider);
        Next();
    }
}

void ThunderBullet::Bullet::SetSpeed(float _speed) {
    speed_ = _speed;
}

void ThunderBullet::Bullet::InitializeNormal() {
    transform_.translate.y = 4.f;
}

void ThunderBullet::Bullet::InitializeChain() {
}

void ThunderBullet::Bullet::UpdateNormal() {
    if (0.7f <= transform_.translate.y){
        transform_.translate.y -= speed_ * deltaTime_;
        return;
    }
    //　床付近で爆散
    if (!isExploded_){
        transform_.translate.y = 0.7f; // 床に着地した位置に調整
        isExploded_ = true;
        pCollider_->Disable();
        explosion_ = std::make_unique<Collision::Collider>();
        explosion_->SetType(Collision::Type::Sphere)
            ->SetTranslate(Adaptor(transform_.translate))
            ->SetSize(3.f)
            ->AddAttribute(static_cast<uint32_t>(Collider::Type::P_BULLET))
            ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
            ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
            ->Enable();

        if (emitter_){
            emitter_->SetEmitterPosition("thunder", transform_.translate);
            emitter_->CreateTemporaryEmitterFrom("thunder", GetUniqueId(), 2.f);
        }
    }

    if (isExploded_){
        if (explosion_ && explosion_->IsEnabled()){
            explosion_->SetTranslate(Adaptor(transform_.translate));
            explosion_->Disable();
            isDead_ = true;
        }
    }
}

void ThunderBullet::Bullet::UpdateChain() {
}

void ThunderBullet::Initialize() {
    BulletBase::Initialize();
    type_ = WeaponType::Thunder;
    speed_ = 0.5f;
    CalcLifeTime();
    if (isChainBullet_){
        InitializeChain();
    } else{
        InitializeNormal();
    }
}

void ThunderBullet::Update() {
    bullet_->Update();
}

void ThunderBullet::Draw() {
    bullet_->Draw();
}

bool ThunderBullet::IsDeadAll() {
    if (!isDead_) return false;
    if (bullet_ && !bullet_->IsDeadAll()) return false;
    if (pNext_)return pNext_->IsDeadAll();
    return true;
}

void ThunderBullet::InitializeNormal() {
    bullet_ = std::make_unique<Bullet>();
    bullet_->SetIsChainBullet(isChainBullet_);
    bullet_->SetChainManager(pChainManager_);
    bullet_->SetTransform(transform_);
    bullet_->SetForward(forward_);
    bullet_->SetSpeed(speed_);
    bullet_->SetEmitter(emitter_);
    bullet_->Initialize();
}

void ThunderBullet::InitializeChain() {
    bullet_ = std::make_unique<Bullet>();
    bullet_->SetIsChainBullet(isChainBullet_);
    bullet_->SetChainManager(pChainManager_);
    bullet_->SetTransform(transform_);
    bullet_->SetForward(forward_);
    bullet_->SetSpeed(speed_);
    bullet_->SetEmitter(emitter_);
    bullet_->Initialize();
}

//DO NOT USE THIS WRAPPER CLASS METHOD
void ThunderBullet::OnCollisionTrigger(const Collision::Collider* _collider) {}
void ThunderBullet::UpdateNormal() {}
void ThunderBullet::UpdateChain() {}

