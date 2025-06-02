#include "FlyEnemy.h"

#include "Object3dBasic.h"
#include "cmath"
#include "GameSystem/DeltaTimeManager/DeltaTimeManager.h"
#include "Type/ColliderType.h"
#include "Utility/Adaptor.h"

void FlyEnemy::Initialize()
{
    EnemyBase::Initialize();

    pGameEventNotifier_ = GameEventNotifier::GetInstance();

    model_ = std::make_unique<Object3d>();
    model_->Initialize();

    model_->SetModel("FlyEnemy.gltf");

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
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetSize(1.f)
        ->SetOwner(this)
        ->Enable();
}

void FlyEnemy::Update()
{
    EnemyBase::Update();

    Object::Update();

    if (isDead_) return;

    Move();

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    model_->Update();

    statusInit_.Update();
    statusCurrent_.Update();
}

void FlyEnemy::Draw()
{
    model_->Draw();
}

void FlyEnemy::Finalize()
{
}

void FlyEnemy::Move()
{
    if (isAppearing_)
    {
        AppearanceProduction();
    }
    else
    {
        Vector3 direction;
        direction = pTarget_->GetTransform().translate - transform_.translate;

        float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
        if (length != 0)
        {
            direction /= length;

            transform_.rotate.y = std::atan2(direction.x, direction.z);
        }

        float deltaTime = DeltaTimeManager::GetInstance()->GetDeltaTime(1);
        transform_.translate += direction * speed_ * deltaTime;

        if (transform_.translate.y < 1.0f)
        {
            transform_.translate.y = 1.0f;
        }
    }
    model_->SetTranslate(transform_.translate);
    model_->SetRotate(transform_.rotate);
}

void FlyEnemy::AppearanceProduction()
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
