#include "Boss.h"

#include "Enemy.h"
#include "imgui.h"
#include "Type/ColliderType.h"
#include <Utility/Adaptor.h>

#include "SceneManager.h"

void Boss::Initialize()
{
    EnemyBase::Initialize();
    hpBarSize_ = {160.0f, 16.0f};
    hpBarOffset_ = {0.0f, 3.5f, 0.0f};

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("boss.gltf", 1, 0);

    // 初期化用 - 外部から設定するためこの値は適用されない
    transform_ = {
        {1.0f,1.0f,1.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,10.0f}
    };
    model_->SetScale(transform_.scale);
    model_->SetRotate(transform_.rotate);
    model_->SetTranslate(transform_.translate);

    pCollider_ = std::make_unique<Collision::Collider>();
    pCollider_
        ->SetEvent(Collision::EventType::Stay, [this](const Collision::Collider* pCol) { OnCollision(pCol); })
        ->SetEvent(Collision::EventType::Trigger, [this](const Collision::Collider* pCol) {OnCollisionTrigger(pCol); })
        ->SetType(Collision::Type::Sphere)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->AddIgnore(static_cast<uint32_t>(Collider::Type::ENEMY))
        ->SetTranslate(Adaptor(transform_.translate))
        ->SetSize(3.f)
        ->SetOwner(this);

    isValid_ = false;

    name_ = "boss";

    statusInit_.setAttack(50)
        .setDefence(0)
        .setHp(500)
        .setMaxHp(500)
        .setSpeed(1)
        .setXpAmount(1000);
    statusCurrent_ = statusInit_;
}

void Boss::Update()
{
    if (!isValid_)return;

    EnemyBase::Update();
    prePos_ = transform_.translate;
    transform_.translate += Vector3{ 0.0f,0.0f,-0.1f };

    pCollider_->SetTranslate(Adaptor(transform_.translate));

    model_->SetTranslate(transform_.translate);
    model_->Update();
}

void Boss::Draw()
{
    if (!isValid_) return;
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

void Boss::SetIsValid(bool isValid) {
    isValid_ = isValid;

    pCollider_->Enable();
}

void Boss::OnCollision(const Collision::Collider* _other) {
    if (isValid_){
        EnemyBase::OnCollision(_other);
    }
}

void Boss::OnCollisionTrigger(const Collision::Collider* _other) {
    if (!isValid_) return;
    if (isDead_ || pCollider_->IsDisabled()) return;

    Object* object = static_cast<Object*>(_other->GetOwner());

    if (_other->GetAttribute() & static_cast<uint32_t>(Collider::Type::P_BULLET)){
        HitEffect();
        Object::StatusUpdateOnCollision(_other);
        pHPBar_->Display(2.0f);
        
        if (statusCurrent_.getHp() <= 0){
            SceneManager::GetInstance()->ChangeScene("clear");    
        }
        
    }

}
