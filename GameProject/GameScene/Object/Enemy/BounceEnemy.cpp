#include "BounceEnemy.h"

#include "Object3dBasic.h"
#include "cmath"
#include "GameSystem/DeltaTimeManager/DeltaTimeManager.h"
#include "Type/ColliderType.h"
#include "Utility/Adaptor.h"

void BounceEnemy::Initialize()
{
    EnemyBase::Initialize();

    pGameEventNotifier_ = GameEventNotifier::GetInstance();

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("bounceEnemy.gltf");

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
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetSize(1.f)
        ->SetOwner(this)
        ->Enable();
}

void BounceEnemy::Update()
{
    EnemyBase::Update();
    if (isDead_) return;

    Move();

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    model_->Update();

    statusCurrent_.Update();
}

void BounceEnemy::Draw()
{
    model_->Draw();
}

void BounceEnemy::Finalize()
{
}

void BounceEnemy::Move()
{
    prePos_ = transform_.translate;
    if (isAppearing_) {
        AppearanceProduction();
    }
    else {
        Vector3 direction = pTarget_->GetTransform().translate - transform_.translate;

        float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
        if (1e-4f < length) {
            direction /= length;

            transform_.rotate.y = std::atan2(direction.x, direction.z);
        }

        transform_.translate.x += direction.x * speed * deltaTime_;
        transform_.translate.z += direction.z * speed * deltaTime_;

        bounceTime_ += 0.05f;
        transform_.translate.y = std::sin(bounceTime_) * bounceHight_ + standardHeight_;

    }
    if (transform_.translate.y <= 1.0f) {
        transform_.translate.y = 1.0f;
        bounceTime_ = 0.0f;
    }
    model_->SetTranslate(transform_.translate);
    model_->SetRotate(transform_.rotate);
}

void BounceEnemy::AppearanceProduction()
{
    float t = appearCounter_ / appearDuration;

    transform_.scale = { t, t, t };
    model_->SetScale(transform_.scale);

    float easeOut = 1.0f - std::pow(1.0f - t, 2.0f);
    transform_.rotate.y = 0 * (1.0f - easeOut) + targetRotate * easeOut * 6.28f;

    model_->SetRotate(transform_.rotate);

    appearCounter_++;
    if (appearCounter_ >= appearDuration) {
        transform_.scale = defaultScale;
        model_->SetScale(transform_.scale);
        transform_.rotate = defaultRotate;
        model_->SetRotate(transform_.rotate);
        isAppearing_ = false;
    }
}
