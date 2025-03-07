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
	model_->SetScale(transform_.scale);
	model_->SetRotate(transform_.rotate);
	model_->SetTranslate(transform_.translate);

	collider_ = std::make_unique<Collider>(this);
	collider_->SetEvent([this](const Object* pObj) {this->OnCollision(pObj); });
}

void Boss::Update()
{
	prePos = transform_.translate;
	model_->Update();
	transform_.translate += Vector3{ 0.0f,0.0f,-0.1f };
	model_->SetTranslate(transform_.translate);
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

void Boss::OnCollision(const Object* pObject)
{
	transform_.translate = prePos;
}
