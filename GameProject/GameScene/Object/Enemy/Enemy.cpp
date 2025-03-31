#include "Enemy.h"

#include "Object3dBasic.h"
#include "cmath"

void Enemy::Initialize()
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

void Enemy::Update()
{
    if (isDead_) return;

    model_->Update();
    Move();
}

void Enemy::Draw()
{
	model_->Draw();
}

void Enemy::Finalize()
{
}

void Enemy::OnCollision(const Collider* pCollider)
{
    if (isDead_) return;

    if (pCollider->GetType() == Collider::Type::ALLY){
        if (0 < hp_){
            hp_--;
        }else{
            isDead_ = true;
            return;
        }

        transform_.translate = prePos_;
        model_->SetTranslate(transform_.translate);
    }
}

void Enemy::Move()
{
    prePos_ = transform_.translate;
    if(isAppearing_) {
        AppearanceProduction();
    }else {
        Vector3 direction;
        direction.x = pTarget_->GetTransform().translate.x - transform_.translate.x;
        direction.z = pTarget_->GetTransform().translate.z - transform_.translate.z;

        float length = std::sqrt(direction.x * direction.x + direction.z * direction.z);
        if (length != 0) {
            direction.x /= length;
            direction.z /= length;
        }

        transform_.translate.x += direction.x * speed_;
        transform_.translate.z += direction.z * speed_;
        
    }
    model_->SetTranslate(transform_.translate);
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
    if (appearCounter_ >= appearDuration_) {
        transform_.scale = defaultScale_;
        model_->SetScale(transform_.scale);
        transform_.rotate = defaultRotate_;
        model_->SetRotate(transform_.rotate);
        isAppearing_ = false;
    }
}
