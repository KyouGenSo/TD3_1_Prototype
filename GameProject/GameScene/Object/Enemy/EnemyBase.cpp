#include "EnemyBase.h"
#include <Type/ColliderType.h>

#include <Camera.h>

#include "Utility/Equals.h"

void EnemyBase::OnCollision(const Collision::Collider* _other) {
    if (isDead_ || pCollider_->IsDisabled()) return;

    Object* object = static_cast<Object*>(_other->GetOwner());

    if (Utility::EqualsIgnoreCase(object->GetName(), "Thunder")){
        StatusUpdateOnCollision(_other);
        if (statusCurrent_.getHp() <= 0){
            pGameEventNotifier_->Notify("EnemyDeadForXP", statusCurrent_.getExperiencePoints());
            pCollider_->Disable();
        }
        pHPBar_->Display(2.0f);
    }
}

void EnemyBase::OnCollisionTrigger(const Collision::Collider* _other)
{
    if (isDead_ || pCollider_->IsDisabled()) return;

    Object* object = static_cast<Object*>(_other->GetOwner());

    if (_other->GetAttribute() & static_cast<uint32_t>(Collider::Type::P_BULLET))
    {
        Object::StatusUpdateOnCollision(_other);
        if (statusCurrent_.getHp() <= 0)
        {
            pGameEventNotifier_->Notify("EnemyDeadForXP", statusCurrent_.getExperiencePoints());
            pCollider_->Disable();
        }
        pHPBar_->Display(2.0f);
    }

    if (Utility::EqualsIgnoreCase(object->GetName(), "Player")){
        pGameEventNotifier_->Notify("PlayerHit", statusCurrent_.getAttack());
        statusCurrent_.setHp(0);
        pCollider_->Disable();
    }
}

void EnemyBase::Initialize()
{
    name_ = "enemy";
    pHPBar_ = std::make_unique<HPBar>();
    pHPBar_->Initialize();
    UpdateHPBarPosition();
}

void EnemyBase::Update()
{
    UpdateHPBarPosition();
    pHPBar_->SetCurrentValue(statusCurrent_.getHp());
    pHPBar_->SetMaxValue(statusCurrent_.getMaxHp());
    pHPBar_->Update();
}

void EnemyBase::Finalize()
{
}

void EnemyBase::UpdateHPBarPosition()
{
    auto camera = Object3dBasic::GetInstance()->GetCamera();

    Vector2 position = HPBar::GetHeadUpPositionOnScreen(
        transform_.translate + hpBarOffset_,
        hpBarSize_,
        *(*camera),
        1.5f
    );

    pHPBar_->SetPosition(position);
    pHPBar_->SetSize(hpBarSize_);
}

void EnemyBase::Draw2d()
{
    pHPBar_->Draw2d();
}

void EnemyBase::HitEffect()
{
    if (emitter_){
        emitter_->SetEmitterPosition("hit", transform_.translate);
        emitter_->CreateTemporaryEmitterFrom("hit", "hit_tmp", 1.f);
    }
}
