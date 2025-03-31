#include "AssaultBullet.h"

#include <numbers>
#include <Quaternion.h>
#include <QuatFunc.h>

void AssaultBullet::Initialize()
{
    BulletBase::Initialize();

    type_ = WeaponType::Assault;
    speed_ = 0.3f;

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("Box.gltf");

    CalcLifeTime();
}

void AssaultBullet::Update()
{
    if (isChainBullet_)
    {
        UpdateChain();
    }
    else
    {
        UpdateNormal();
    }

    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();

    if (pNext_)
    {
        pNext_->Update();
    }

    isDead_ = CheckLifeTime();
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

void AssaultBullet::OnCollisionTrigger(const Collider* _other)
{
    Next();
}

void AssaultBullet::InitializeNormal()
{
    bullet_ = std::make_unique<Bullet>();
    bullet_->Initialize();
    bullet_->SetPosition(transform_.translate);
    bullet_->SetRotate(transform_.rotate);
    bullet_->SetForward(forward_);
    bullet_->SetSpeed(speed_);
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
        bullets_[i]->Initialize();
        bullets_[i]->SetPosition(position);
        bullets_[i]->SetForward(forward_);
        bullets_[i]->SetSpeed(speed_);
    }
}

void AssaultBullet::UpdateNormal()
{
    bullet_->Update();
}

void AssaultBullet::UpdateChain()
{
    for (const auto& bullet : bullets_)
    {
        if (!bullet) break;
        bullet->Update();
    }
}
