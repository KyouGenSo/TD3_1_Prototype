#include "MachineGunBullet.h"
#include "GameSystem/Reinforcement/Manager/ReinforcementManager.h"
#include "Type/ColliderType.h"
#include "Utility/Adaptor.h"

void MachineGunBullet::Bullet::Initialize() {
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

void MachineGunBullet::Bullet::Update() {
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

void MachineGunBullet::Bullet::Draw() {
    if (isDead_ && !pNext_) return;
    model_->Draw();
    
    if (pNext_){
        pNext_->Draw();
    }
}

void MachineGunBullet::Bullet::OnCollisionTrigger(const Collision::Collider* _collider) {
    if (isDead_ || pCollider_->IsDisabled()) return;

    if (_collider->GetAttribute() & static_cast<uint32_t>(Collider::Type::ENEMY)){
        pCollider_->Disable();
        isDead_ = true;

        ReinforcementManager::GetInstance()->Notify("onHit");

        Next();
    }
}

void MachineGunBullet::Bullet::InitializeNormal() {
    statusInit_.setAttack(7)
        .setHp(1)
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(1);
    statusCurrent_ = statusInit_;
}

void MachineGunBullet::Bullet::InitializeChain() {
    statusInit_.setAttack(7)
        .setHp(1)
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(1);
    statusCurrent_ = statusInit_;
}

void MachineGunBullet::Bullet::UpdateNormal() {
    transform_.translate += forward_ * speed_;
    pCollider_->SetTranslate(Adaptor(transform_.translate));
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void MachineGunBullet::Bullet::UpdateChain() {
    transform_.translate += forward_ * speed_;
    pCollider_->SetTranslate(Adaptor(transform_.translate));
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

