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

    collider_ = std::make_unique<Collider>(this);
    collider_->SetEvent([this](const Collider* pObj) {this->OnCollision(pObj); });
    collider_->SetType(Collider::Type::ENEMY);
    collider_->SetIgnore(Collider::Type::ENEMY);
    collider_->SetIgnore(Collider::Type::STAGE);
    collider_->SetSize(1.f);
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

void FlyEnemy::OnCollision(const Collider* pCollider)
{
    if (isDead_) return;

    if (pCollider->GetType() == Collider::Type::ALLY) {
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

        transform_.translate += direction * speed_;
        if (transform_.translate.y < 1.0f) {
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
    if (appearCounter_ >= appearDuration_) {
        transform_.scale = defaultScale_;
        model_->SetScale(transform_.scale);
        transform_.rotate = defaultRotate_;
        model_->SetRotate(transform_.rotate);
        isAppearing_ = false;
    }
}
