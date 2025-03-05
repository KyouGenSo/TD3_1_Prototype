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
        {0.0f,0.0f,0.0f}
    };
}

void Enemy::Update()
{
	model_->Update();
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

void Enemy::OnCollisionTrigger(const Object* eObject)
{
}
