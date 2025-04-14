#include "Enemy.h"

#include "Object3dBasic.h"
#include "cmath"
#include "Type/ColliderType.h"

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

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_->SetEvent(Collision::EventType::Stay, [this](const Collision::Collider* pObj) {this->OnCollision(pObj); })
        ->SetType(Collision::Type::Sphere)
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetSize(1.f)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::STAGE))
        ->Enable();
}

void Enemy::Update()
{
    if (isDead_) return;

    Move();

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    model_->Update();
}

void Enemy::Draw()
{
	model_->Draw();
}

void Enemy::Finalize()
{
}

void Enemy::OnCollision(const Collision::Collider* pCollider)
{
    if (isDead_) return;

    if (pCollider->GetAttribute() & static_cast<uint32_t>(Collider::Type::ALLY) || pCollider->GetAttribute() & static_cast<uint32_t>(Collider::Type::P_BULLET)){
        if (0 < hp_){
            hp_--;
        }else{
            isDead_ = true;
            return;
        }

        transform_.translate = prePos;
        model_->SetTranslate(transform_.translate);
    }
}

void Enemy::Move()
{
    prePos = transform_.translate;
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

        transform_.translate.x += direction.x * speed;
        transform_.translate.z += direction.z * speed;
        
    }
    model_->SetTranslate(transform_.translate);
}

void Enemy::AppearanceProduction()
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
