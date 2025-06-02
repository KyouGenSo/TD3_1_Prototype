#include "AssaultBullet.h"

#include <numbers>
#include <Quaternion.h>
#include <QuatFunc.h>

void AssaultBullet::Initialize()
{
    BulletBase::Initialize();

    type_ = WeaponType::Assault;
    speed_ = 60.0f;

    CalcLifeTime();
}

void AssaultBullet::Update()
{
    BulletBase::Update();

    if (isChainBullet_)
    {
        UpdateChain();
    }
    else
    {
        UpdateNormal();
    }

    if (pNext_)
    {
        pNext_->Update();
    }

    isDead_ = CheckLifeTime();

    if (!pChainManager_){
        __debugbreak();
    }
}

void AssaultBullet::Draw()
{
    if (isDead_) return;
    if (isChainBullet_)
    {
        for (const auto& bullet : bullets_)
        {
            if (!bullet) break;
            bullet->Draw();
        }
    }
    else
    {
        bullet_->Draw();
    }
}

bool AssaultBullet::IsDeadAll() {
    if (!isDead_) return false;

    if (isChainBullet_){
        for (const auto& bullet : bullets_){
            if (!bullet) break;
            if (!bullet->IsDeadAll()) return false;
        }
    }
    if (bullet_ && !bullet_->IsDeadAll()) return false;

    return true;
}

void AssaultBullet::InitializeNormal()
{
    bullet_ = std::make_unique<Bullet>();
    bullet_->SetPosition(transform_.translate);
    bullet_->SetRotation(transform_.rotate);
    bullet_->SetForward(forward_);
    bullet_->SetSpeed(speed_);
    bullet_->SetChainManager(pChainManager_);
    bullet_->SetEmitter(emitter_);
    bullet_->Initialize();
}

void AssaultBullet::InitializeChain()
{
    Vector3 position = transform_.translate;
    position.y = min(0.5f, position.y);

    for (int i = 0; i < bullets_.size(); i++)
    {
        float angle = std::numbers::pi_v<float> / static_cast<float>(bullets_.size()) * i * 2.0f;

        Quaternion yaw = Quat::MakeRotateAxisAngle({ 0.0f, 1.0f, 0.0f }, angle);
        Quaternion pitch = Quat::MakeRotateAxisAngle({ 1.0f, 0.0f, 0.0f }, transform_.rotate.x);

        Quaternion rotate = yaw * pitch;

        Vector3 forward_ = Quat::RotateVec3({ 0.0f, 0.0f, 1.0f }, rotate);

        bullets_[i] = std::make_unique<Bullet>();
        bullets_[i]->SetPosition(position);
        bullets_[i]->SetForward(forward_);
        bullets_[i]->SetSpeed(speed_);
        bullets_[i]->SetChainManager(pChainManager_);
        bullets_[i]->Initialize();
    }
}

void AssaultBullet::UpdateNormal()
{
    bullet_->Update();

    if (bullet_->IsDead() && !isDead_)
    {
        isDead_ = true;
    }
}

void AssaultBullet::UpdateChain()
{
    for (const auto& bullet : bullets_)
    {
        if (!bullet) break;
        bullet->Update();
    }
}
