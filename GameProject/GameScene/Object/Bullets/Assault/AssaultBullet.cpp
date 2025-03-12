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

    if (pNextBulletTimer_->GetNow() > 1.0 && pNextBulletTimer_->GetIsStart())
    {
        pNextBulletTimer_->Reset();

        if (pChainManager_->IsLastWeapon(type_)) return;

        // クールタイムの確認
        if (BulletBase::CheckCoolTime() == false) return;

        // 次の弾の生成
        BulletBase::CreateNextBullet();

        // 次の弾の発射
        pNext_->Fire();
    }

    if (pNext_)
    {
        pNext_->Update();
    }

    isDead_ =  BulletBase::CheckLifeTime();
}

void AssaultBullet::Draw()
{
    if (isDead_) return;
    if (isChainBullet_)
    {
        for (int i = 0; i < bullets_.size(); i++)
        {
            bullets_[i]->Draw();
        }
    }
    else
    {
        bullet_->Draw();
    }
}

void AssaultBullet::Fire()
{
    BulletBase::Fire();
    pChainManager_->OnAttacked(type_);
}

void AssaultBullet::OnCollisionTrigger(const Object* _other)
{
}

void AssaultBullet::InitializeNormal()
{
    bullet_ = std::make_unique<Bullet>();
    bullet_->Initialize();
    bullet_->SetPosition(transform_.translate);
    bullet_->SetForward(Vector3(0, 0, 1));
    bullet_->SetSpeed(speed_);
}

void AssaultBullet::InitializeChain()
{
    float angle = 0;
    for (int i = 0; i < bullets_.size(); i++)
    {
        angle = std::numbers::pi_v<float> / bullets_.size() * i * 2.0f;

        Quaternion yaw = Quat::MakeRotateAxisAngle({ 0.0f, 1.0f, 0.0f }, angle);
        Quaternion pitch = Quat::MakeRotateAxisAngle({ 1.0f, 0.0f, 0.0f }, transform_.rotate.x);

        Quaternion rotate = yaw * pitch;

        Vector3 forward_ = Quat::RotateVec3({ 0.0f, 0.0f, 1.0f }, rotate);

        bullets_[i] = std::make_unique<Bullet>();
        bullets_[i]->Initialize();
        bullets_[i]->SetPosition(transform_.translate);
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
    for (int i = 0; i < bullets_.size(); i++)
    {
        bullets_[i]->Update();
    }
}
