#include "Enemy.h"
#include "Object3dBasic.h"

void Enemy::Initialize()
{
    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    eCamera_ = Object3dBasic::GetInstance()->GetCamera();
    model_->SetCamera(eCamera_);
    model_->SetModel("cube.gltf");

    transform_ = {
        {1.0f,1.0f,1.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,5.0f}
    };
	model_->SetScale(transform_.scale);
	model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);

    collider_ = std::make_unique<Collider>(this);
    collider_->SetEvent([this](const Object* pObj) {this->OnCollision(pObj); });
}

void Enemy::Update()
{
    prePos = transform_.translate;
    model_->Update();
    transform_.translate += Vector3{0.0f,0.0f,-0.1f};
	model_->SetTranslate(transform_.translate);

}

void Enemy::Draw()
{
	model_->Draw();
}

void Enemy::Finalize()
{
}

bool Enemy::IsDead()
{
	return false;
}

void Enemy::OnCollision(const Object* pObject)
{
	transform_.translate = prePos;
}
