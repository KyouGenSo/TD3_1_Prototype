#include "Enemy.h"

#include "cmath"
#include "Type/ColliderType.h"
#include <Utility/Adaptor.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>

void Enemy::Initialize()
{
    EnemyBase::Initialize();

    pGameEventNotifier_ = GameEventNotifier::GetInstance();

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("normalEnemy.gltf",1,0);

    transform_ = {
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f}
    };
    model_->SetScale(transform_.scale);
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);

    statusInit_.setAttack(20)
        .setDefence(0)
        .setHp(20)
        .setMaxHp(20)
        .setSpeed(1)
        .setXpAmount(20);
    statusCurrent_ = statusInit_;

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_
        ->SetEvent(Collision::EventType::Stay, [this](const Collision::Collider* pObj) { EnemyBase::OnCollision(pObj); })
        ->SetEvent(Collision::EventType::Trigger, [this](const Collision::Collider* pObj) { EnemyBase::OnCollisionTrigger(pObj); })
        ->SetType(Collision::Type::Sphere)
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetSize(1.f)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetOwner(this)
        ->Enable();
}

void Enemy::Update()
{
    EnemyBase::Update();

    if (isDead_) return;

    Move();

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    model_->Update();

    statusCurrent_.Update();
}

void Enemy::Draw()
{
    model_->Draw();
}

void Enemy::Finalize()
{
}

void Enemy::Move()
{
    prePos_ = transform_.translate;
    if (isAppearing_)
    {
        AppearanceProduction();
    }
    else
    {
        Vector3 direction;
        direction.x = pTarget_->GetTransform().translate.x - transform_.translate.x;
        direction.z = pTarget_->GetTransform().translate.z - transform_.translate.z;

        float length = std::sqrt(direction.x * direction.x + direction.z * direction.z);
        if (1e-4f < length)
        {
            direction.x /= length;
            direction.z /= length;

            transform_.rotate.y = std::atan2(direction.x, direction.z);
        }

        transform_.translate.x += direction.x * speed_ * deltaTime_;
        transform_.translate.z += direction.z * speed_ * deltaTime_;

    }
    model_->SetTranslate(transform_.translate);
    model_->SetRotate(transform_.rotate);
}

void Enemy::AppearanceProduction()
{
    float t = appearCounter_ / appearDuration_;

    transform_.scale = { t, t, t };
    model_->SetScale(transform_.scale);

    float easeOut = 1.0f - std::pow(1.0f - t, 2.0f);
    transform_.rotate.y = 0 * (1.0f - easeOut) + targetRotate_ * easeOut * 6.28f;

    model_->SetRotate(transform_.rotate);

    appearCounter_++;
    if (appearCounter_ >= appearDuration_)
    {
        transform_.scale = defaultScale_;
        model_->SetScale(transform_.scale);
        transform_.rotate = defaultRotate_;
        model_->SetRotate(transform_.rotate);
        isAppearing_ = false;
    }
}
