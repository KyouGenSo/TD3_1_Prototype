#include "RocketBullet.h"

#include <Quaternion.h>
#include <QuatFunc.h>

void RocketBullet::Initialize()
{
    type_ = WeaponType::RocketLauncher;
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("AnimatedCube.gltf");
}

void RocketBullet::Update()
{
    transform_.translate += forward_ * speed_;
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void RocketBullet::Draw()
{
    // 描画処理
    model_->Draw();
}

void RocketBullet::Fire()
{
    Bullet::Fire();
    pChainManager_->OnAttacked(type_); // チェインマネージャーに攻撃されたことを通知

    float coolTime = 0;
    if (pChainManager_->IsLastWeapon(type_)) return;

    // クールタイムの確認
    if (Bullet::CheckCoolTime() == false) return;

    // 次の弾の生成
    CreateNextBullet();

    // 次の弾の発射
    pNext_->Fire();
}

void RocketBullet::OnCollisionTrigger(const Object* _other)
{

}

void RocketBullet::AttackNormal()
{
    Quaternion yaw = Quat::MakeRotateAxisAngle({0.0f, 1.0f, 0.0f}, transform_.rotate.y);
    Quaternion pitch = Quat::MakeRotateAxisAngle({ 1.0f, 0.0f, 0.0f }, transform_.rotate.x);

    Quaternion rotate = yaw * pitch;

    forward_ = Quat::RotateVec3({ 0.0f, 0.0f, 1.0f }, rotate);
}

void RocketBullet::AttackChain()
{
}
