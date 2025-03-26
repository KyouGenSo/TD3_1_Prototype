#include "MachineGunBullet.h"

void MachineGunBullet::Initialize() {
    BulletBase::Initialize();

    type_ = WeaponType::MachineGun;
    speed_ = 0.9f;

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("AnimatedCube.gltf");
    model_->SetScale({0.2f, 0.2f, 0.2f});

    CalcLifeTime();

    pCollider_ = std::make_unique<Collider>(this);
    pCollider_
        ->SetEvent([&](const Collider* pCol){ OnCollisionTrigger(pCol); })
        ->SetSize(0.2f)
        ->SetType(Collider::Type::ALLY)
        ->SetIgnore(Collider::Type::ALLY)
        ->SetIgnore(Collider::Type::STAGE);
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
}

void MachineGunBullet::Draw() {
    if (!isDead_){
        model_->Draw();
    }
    if (pNext_){
        pNext_->Draw();
    }
}

void MachineGunBullet::OnCollisionTrigger(const Collider* _collider) {
    if(isDead_ || pCollider_->IsDisable())return;

    isDead_ = true;
    pCollider_->Disable();

    Next();
}

void MachineGunBullet::InitializeNormal() {
}

void MachineGunBullet::InitializeChain() {
}

void MachineGunBullet::UpdateNormal() {
    transform_.translate += forward_ * speed_;

    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void MachineGunBullet::UpdateChain() {
}
