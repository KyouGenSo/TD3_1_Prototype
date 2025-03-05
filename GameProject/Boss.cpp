#include "Boss.h"
#include "Object3dBasic.h"

void Boss::Initialize()
{
	model_ = std::make_unique<Object3d>();
	model_->Initialize();
	bCamera_ = Object3dBasic::GetInstance()->GetCamera();
	model_->SetCamera(bCamera_);
	model_->SetModel("bigCube.gltf");
	transform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,10.0f}
	};
	model_->SetTranslate(transform_.translate);
}

void Boss::Update()
{
	model_->Update();
}

void Boss::Draw()
{
	model_->Draw();
}

void Boss::Finalize()
{
}

bool Boss::IsDead()
{
	return false;
}

void Boss::OnCollisionTrigger(const Object* bObject)
{
}
