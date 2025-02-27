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
        {1,1,1},
        {0,0.f,0},
        {0,0,0}
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
