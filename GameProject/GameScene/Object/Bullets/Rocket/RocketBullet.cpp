#include "RocketBullet.h"

#include <Quaternion.h>
#include <QuatFunc.h>

#include "Collision/Collider.h"

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

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_->SetEvent(Collision::EventType::Trigger, [this](const Collision::Collider* pCol){this->OnCollisionTrigger(pCol); })
        ->SetSize(0.4f)
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->Enable();
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
    if (!isDead_){
        // 描画処理
        model_->Draw();
    }

    if (pNext_) pNext_->Draw();
}

void RocketBullet::OnCollisionTrigger(const Collision::Collider* _other)
{
    if (isDead_ || pCollider_->IsDisabled()) return;

    isDead_ = true;
    pCollider_->Disable();

    color = {1, 0,0,1};
    //爆発オブジェクトを生成
    explosion_ = std::make_unique<Collision::Collider>();
    explosion_->SetSize(5.0f)
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->Enable();

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
