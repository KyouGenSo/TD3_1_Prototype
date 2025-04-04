#include "FlyEnemy.h"

#include "Object3dBasic.h"
#include "cmath"

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

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_->SetEvent(Collision::EventType::Stay, [this](const Collision::Collider* pObj) {this->OnCollision(pObj); })
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetSize(1.f)
        ->Enable();
}

void FlyEnemy::Update()
{
    if (isDead_) return;

    model_->Update();
    Move();
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

    if (pCollider->GetAttribute() & static_cast<uint32_t>(Collider::Type::ALLY)) {
        if (0 < hp_) {
            hp_--;
        }
        else {
            isDead_ = true;
            return;
        }

        model_->SetTranslate(transform_.translate);
    }
}

void FlyEnemy::Move()
{
    if (isAppearing_) {
        AppearanceProduction();
    }
    else {
        Vector3 direction;
        direction = pTarget_->GetTransform().translate - transform_.translate;

        float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
        if (length != 0) {
            direction /= length;
        }

        transform_.translate += direction * speed;
        if (transform_.translate.y < 1.0f) {
            transform_.translate.y = 1.0f;
        }
    }
    model_->SetTranslate(transform_.translate);
}

void FlyEnemy::AppearanceProduction()
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
