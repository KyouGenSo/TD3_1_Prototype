#include "AssaultBullet.h"

#include <ModelManager.h>

#include "QuatFunc.h"
#include "Type/ColliderType.h"
#include <GameSystem/Reinforcement/Manager/ReinforcementManager.h>
#include <Utility/Adaptor.h>

void AssaultBullet::Bullet::Initialize()
{
    BulletBase::Initialize();

    type_ = WeaponType::Assault;

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("assultBullet.gltf");
    model_->SetScale(Vector3(1.0f, 1.0f, 1.0f));

    statusInit_
        .setAttack(7)
        .setHp(1)
        .setSpeed(1)
        .setDefence(0)
        .setMaxHp(1);
    statusCurrent_ = statusInit_;

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_
        ->SetEvent(Collision::EventType::Trigger, [this]<typename T0>(T0&& PH1) {
            OnCollisionTrigger(std::forward<T0>(PH1));
        })
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetSize(0.3f)
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ALLY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::P_BULLET))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetOwner(this)
        ->Enable();

    speed_ = 80.0f;

    origin = transform_.translate;

    Update();
}

void AssaultBullet::Bullet::Update()
{
    BulletBase::Update();

    isDead_ = CheckLifeTime();

    transform_.translate += forward_ * speed_ * deltaTime_;

    if (50.f <= (transform_.translate - origin).Length()){
        isDead_ = true;
        pCollider_->Disable();
    }

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);
    model_->Update();

    if (pNext_){
         pNext_->Update();
    }
}

void AssaultBullet::Bullet::Draw()
{
    if (model_ && !isDead_)
    {
        model_->Draw();
    }

    if (pNext_)
    {
        pNext_->Draw();
    }
}

void AssaultBullet::Bullet::OnCollisionTrigger(const Collision::Collider* _other)
{
    if (_other->IsDisabled()) return;

    if (_other->GetAttribute() & static_cast<uint32_t>(Collider::Type::ENEMY))
    {
        pCollider_->Disable();
        isDead_ = true;
        //敵に当たった場合
        ReinforcementManager::GetInstance()->Notify("onHit");

        Next();
    }
}

void AssaultBullet::Bullet::InitializeNormal()
{
}

void AssaultBullet::Bullet::InitializeChain()
{
}

void AssaultBullet::Bullet::UpdateNormal()
{
}

void AssaultBullet::Bullet::UpdateChain()
{
}
