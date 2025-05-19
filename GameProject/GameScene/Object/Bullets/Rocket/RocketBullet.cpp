#include "RocketBullet.h"

#include <Quaternion.h>
#include <QuatFunc.h>

#include "EmitterManager.h"
#include "ModelManager.h"
#include "Collision/Collider.h"
#include "Type/ColliderType.h"
#include <Utility/Adaptor.h>

void RocketBullet::Initialize()
{
    BulletBase::Initialize();

    type_ = WeaponType::RocketLauncher;
    speed_ = 0.4f;

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("rocketBullet.gltf");
    model_->SetScale({ 0.4f, 0.4f, 0.4f });

    CalcLifeTime();

    statusInit_
        .setAttack(5)
        .setHp(1)
        .setLevel(1)
        .setExp(0)
        .setMaxExp(1)
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(1);
    statusCurrent_ = statusInit_;

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_->SetEvent(Collision::EventType::Trigger, [this](const Collision::Collider* pCol){this->OnCollisionTrigger(pCol); })
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetSize(0.3f)
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->SetOwner(this)
        ->Enable();
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
    if (!isDead_)
    {
        // 描画処理
        model_->Draw();
    }

    if (pNext_) pNext_->Draw();
    Object::DrawCollider(pCollider_.get());
}

void RocketBullet::OnCollisionTrigger(const Collision::Collider* _other)
{
    if (isDead_ || pCollider_->IsDisabled()) return;

    isDead_ = true;
    pCollider_->Disable();

    // 強化カードの効果を適用するための通知
    NotifyReinforcementManager(_other);

    if (emitter_){
        emitter_->SetEmitterPosition("explosion", transform_.translate);
        emitter_->CreateTemporaryEmitterFrom("explosion", "tmp", 2.f);
        explode_ = true;
    }


    //爆発オブジェクトを生成
    explosion_ = std::make_unique<Collision::Collider>();
    explosion_
        ->SetType(Collision::Type::Sphere)
        ->SetTranslate(Adaptor(explosionPos_))
        ->SetSize(5.0f)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetOwner(this)
        ->Enable();

    Next();
}

void RocketBullet::InitializeNormal()
{
    Quaternion yaw = Quat::MakeRotateAxisAngle({ 0.0f, 1.0f, 0.0f }, transform_.rotate.y);
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

    pCollider_->SetTranslate(Adaptor(transform_.translate));
}

void RocketBullet::UpdateChain()
{
}
