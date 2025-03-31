#include "Boss.h"

#include "Enemy.h"
#include "imgui.h"
#include "Object3dBasic.h"

void Boss::Initialize()
{
	model_ = std::make_unique<Object3d>();
	model_->Initialize();
	pCamera_ = Object3dBasic::GetInstance()->GetCamera();
	model_->SetCamera(pCamera_);
	model_->SetModel("bigCube.gltf");

    // 初期化用 - 外部から設定するためこの値は適用されない
	transform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,10.0f}
	};
	model_->SetScale(transform_.scale);
	model_->SetRotate(transform_.rotate);
	model_->SetTranslate(transform_.translate);

	collider_ = std::make_unique<Collider>(this);
	collider_->SetEvent([this](const Collider* pCol) {this->OnCollision(pCol); });
    collider_->SetType(Collider::Type::ENEMY);
    collider_->SetIgnore(Collider::Type::ENEMY);

    isValid_ = false;
}

void Boss::Update()
{
    if(isValid_){
	    prePos_ = transform_.translate;
	    transform_.translate += Vector3{ 0.0f,0.0f,-0.1f };
    }else {
        
    }
    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void Boss::Draw()
{
	model_->Draw();
}

void Boss::Finalize()
{
}

void Boss::ImGui()
{
    ImGui::Begin("Boss");
    ImGui::Checkbox("isValid", &isValid_);
    ImGui::End();
}

void Boss::OnCollision(const Collider* pCollider)
{
	transform_.translate = prePos_;
}
