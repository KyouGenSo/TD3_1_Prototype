#include "FlyEnemy.h"

#include "Object3dBasic.h"
#include "cmath"
#include "Type/ColliderType.h"
#include <Utility/Adaptor.h>

void FlyEnemy::Initialize()
{
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    pCamera_ = Object3dBasic::GetInstance()->GetCamera();
    model_->SetCamera(pCamera_);
    model_->SetModel("cube.gltf");

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
        .setExp(0)
        .setMaxExp(1)
        .setLevel(0)
        .setSpeed(1);
    statusCurrent_ = statusInit_;

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_->SetEvent(Collision::EventType::Stay, [this](const Collision::Collider* pObj) { this->OnCollision(pObj); })
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
    Object::Update();

    if (isDead_) return;

    Move();

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    model_->Update();
}

void FlyEnemy::Draw()
{
    model_->Draw();
}

void FlyEnemy::Finalize()
{
}

void FlyEnemy::OnCollision(const Collision::Collider* pCollider)
{
    if (isDead_) return;

    if (pCollider->GetAttribute() & static_cast<uint32_t>(Collider::Type::ALLY))
    {
        Object::StatusUpdateOnCollision(pCollider);
    }
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
        }

        transform_.translate += direction * speed_;
        if (transform_.translate.y < 1.0f)
        {
            transform_.translate.y = 1.0f;
        }
    }
    model_->SetTranslate(transform_.translate);
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
