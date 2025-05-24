#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "MachineGunBullet.h"

#include <numbers>
#include <Collision/Collider.h>
#include <Utility/Adaptor.h>

void MachineGunBullet::Initialize() {
    BulletBase::Initialize();

    type_ = WeaponType::MachineGun;
    speed_ = 0.9f;

    if (isChainBullet_){
        InitializeChain();
    } else{
        InitializeNormal();
    }

    CalcLifeTime();
}

void MachineGunBullet::Update()
{
    for (auto& bullet : bullets_){
        bullet->Update();
    }
    isDead_ = CheckLifeTime();
}

void MachineGunBullet::Draw()
{
    if (isDead_ && !pNext_)return;

    for (const auto& bullet : bullets_)
    {
        bullet->Draw();
    }
}

void MachineGunBullet::OnCollisionTrigger(const Collision::Collider* _collider) {
    if(isDead_ || pCollider_->IsDisabled())return;

    isDead_ = true;
    pCollider_->Disable();

    Next();
}

void MachineGunBullet::InitializeNormal()
{
    bullets_.resize(1);
    bullets_[0] = std::make_unique<Bullet>();
    bullets_[0]->Initialize();
    bullets_[0]->SetTransform(transform_);
    bullets_[0]->SetForward(forward_);
    bullets_[0]->SetSpeed(speed_);
    bullets_[0]->SetChainManager(pChainManager_);
    bullets_[0]->SetEmitter(emitter_);
    bullets_[0]->SetIsChainBullet(isChainBullet_);
}

void MachineGunBullet::InitializeChain()
{
    Vector3 pos = transform_.translate;
    pos.y = std::min(0.5f, pos.y);
    bullets_.resize(32);

    for (size_t i = 0; i < bullets_.size(); i++)
    {
        float angle = std::numbers::pi_v<float> / static_cast<float>(bullets_.size()) * static_cast<float>(i) * 2.0f;
        bullets_[i] = std::make_unique<Bullet>();
        bullets_[i]->SetIsChainBullet(true);
        bullets_[i]->Initialize();
        bullets_[i]->SetTransform({.scale= {.x= 1, .y= 1, .z= 1}, .rotate= {0.f, angle, 0.f}, .translate= pos});
        bullets_[i]->SetForward({.x= sin(angle), .y= 0.0f, .z= cos(angle)});
        bullets_[i]->SetSpeed(speed_);
    }
}

void MachineGunBullet::UpdateNormal()
{
    std::erase_if(bullets_, [&](const auto& bullet){ return bullet->IsDead(); });
    for (const auto& bullet : bullets_){
        bullet->Update();
    }
}

void MachineGunBullet::UpdateChain()
{
    std::erase_if(bullets_, [&](const auto& b) { return b->IsDead(); });
    for (const auto& bullet : bullets_)
    {
        bullet->Update();
    }
}

bool MachineGunBullet::IsDeadAll() {
    if (!isDead_) return false;
    for (const auto& bullet : bullets_){
        if (!bullet) break;
        if (!bullet->IsDeadAll()) return false;
    }
    return true;
}
