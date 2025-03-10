#include "RocketBullet.h"

#include <Quaternion.h>
#include <QuatFunc.h>

void RocketBullet::Initialize()
{
    BulletBase::Initialize();
    
    type_ = WeaponType::RocketLauncher;
    speed_ = 0.4f;

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("AnimatedCube.gltf");

    CalcLifeTime();
}

void RocketBullet::Update()
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
    isDead_ = CheckLifeTime();
}

void RocketBullet::Draw()
{
    // 描画処理
    model_->Draw();

    if (pNext_) pNext_->Draw();
}

void RocketBullet::Fire()
{
    BulletBase::Fire();
    pChainManager_->OnAttacked(type_); // チェインマネージャーに攻撃されたことを通知
}

void RocketBullet::OnCollisionTrigger(const Object* _other)
{

}

void RocketBullet::AttackNormalInitialize()
{

}

void RocketBullet::AttackChainInitialize()
{
}

void RocketBullet::UpdateNormal()
{
    Quaternion yaw = Quat::MakeRotateAxisAngle({0.0f, 1.0f, 0.0f}, transform_.rotate.y);
    Quaternion pitch = Quat::MakeRotateAxisAngle({ 1.0f, 0.0f, 0.0f }, transform_.rotate.x);

    Quaternion rotate = yaw * pitch;

    forward_ = Quat::RotateVec3({ 0.0f, 0.0f, 1.0f }, rotate);
    transform_.translate += forward_ * speed_;
}

void RocketBullet::UpdateChain()
{
}
