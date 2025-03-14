#include "Enemy.h"

#include "imgui.h"
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
    collider_->SetEvent([this](const Object* pObj) {this->OnCollision(pObj); });
}

void Enemy::Update()
{
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

void Enemy::OnCollision(const Object* pObject)
{
	transform_.translate = prePos;
}

void Enemy::Move()
{
    prePos = transform_.translate;
    if(isAppearing_) {
        AppearanceProduction();
    }else {
        Vector3 direction = pTarget_->GetTransform().translate - transform_.translate;
        direction /= direction.Length();
        transform_.translate += direction * speed; 
        
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
