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
    BulletBase::Update();

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
    if (!isDead_){
        // 描画処理
        model_->Draw();
    }

    if (pNext_) pNext_->Draw();
}

void RocketBullet::OnCollisionTrigger(const Collider* _other)
{
    if (isDead_ || pCollider_->IsDisable()) return;

    isDead_ = true;
    pCollider_->Disable();

    color = {1, 0,0,1};
    //爆発オブジェクトを生成
    explosion_ = std::make_unique<LimitedCollider>(this, 1);
    explosion_->SetSize(5.0f);
    explosion_->SetType(Collider::Type::ALLY);
    explosion_->SetIgnore(Collider::Type::STAGE);

    Next();
}

void RocketBullet::InitializeNormal()
{
    Quaternion yaw = Quat::MakeRotateAxisAngle({0.0f, 1.0f, 0.0f}, transform_.rotate.y);
    Quaternion pitch = Quat::MakeRotateAxisAngle({ 1.0f, 0.0f, 0.0f }, transform_.rotate.x);

    Quaternion rotate = yaw * pitch;

    forward_ = Quat::RotateVec3({ 0.0f, 0.0f, 1.0f }, rotate);
}

void RocketBullet::InitializeChain()
{
}

void RocketBullet::UpdateNormal()
{
    transform_.translate += forward_ * speed_;
}

void RocketBullet::UpdateChain()
{
}
