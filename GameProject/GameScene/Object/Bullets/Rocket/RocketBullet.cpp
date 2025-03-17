#include "RocketBullet.h"

#include <Quaternion.h>
#include <QuatFunc.h>

#include "GameScene/Object/Collision/LimitedCollider.h"

void RocketBullet::Initialize()
{
    BulletBase::Initialize();
    
    type_ = WeaponType::RocketLauncher;
    speed_ = 0.4f;

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("AnimatedCube.gltf");
    model_->SetScale({0.4f, 0.4f, 0.4f});

    CalcLifeTime();

    pCollider_ = std::make_unique<Collider>(this);
    pCollider_->SetEvent([this](const Collider* pCol){this->OnCollisionTrigger(pCol); }, Collider::Event::TRIGGER);
    pCollider_->SetSize(0.4f);
    pCollider_->SetType(Collider::Type::ALLY);
    pCollider_->SetIgnore(Collider::Type::ALLY);
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

    if (pNext_)
    {
        pNext_->Update();
    }
    isDead_ = CheckLifeTime();
}

void RocketBullet::Draw()
{
    // 描画処理
    model_->SetMaterialColor(color);
    model_->Draw();
    model_->SetMaterialColor({1,1,1,1});

    if (pNext_) pNext_->Draw();
}

void RocketBullet::Fire()
{
    BulletBase::Fire();
    pChainManager_->OnAttacked(type_); // チェインマネージャーに攻撃されたことを通知
}

void RocketBullet::OnCollisionTrigger(const Collider* _other)
{
    pCollider_->Disable();

    color = {1, 0,0,1};
    //爆発オブジェクトを生成
    explosion_ = std::make_unique<LimitedCollider>(this, 1);
    explosion_->SetSize(5.0f);

    if(pNextBulletTimer_->GetIsStart())
    {
        pNextBulletTimer_->Reset();

        if (pChainManager_->IsLastWeapon(type_)) return;

        // クールタイムの確認
        if (BulletBase::CheckCoolTime() == false) return;

        // 次の弾の生成
        BulletBase::CreateNextBullet();

        // 次の弾の発射
        pNext_->Fire();
        pNext_->Update();
    }
}

void RocketBullet::InitializeNormal()
{

}

void RocketBullet::InitializeChain()
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
