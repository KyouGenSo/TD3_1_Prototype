#include "EnemyBase.h"
#include <Type/ColliderType.h>

#include <Camera.h>

#include "GameSystem/DeltaTimeManager/DeltaTimeManager.h"
#include "Utility/Equals.h"

void EnemyBase::OnCollision(const Collision::Collider* _other) {
    if (isDead_ || pCollider_->IsDisabled()) return;

    Object* object = static_cast<Object*>(_other->GetOwner());

    if (Utility::EqualsIgnoreCase(object->GetName(), "Thunder")){
        HitEffect();
        StatusUpdateOnCollision(_other);
        if (statusCurrent_.getHp() <= 0){
            pGameEventNotifier_->Notify("EnemyDeadForXP", statusCurrent_.getExperiencePoints());
            pCollider_->Disable();
            isDead_ = true;
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
        HitEffect();
        Object::StatusUpdateOnCollision(_other);
        pHPBar_->Display(2.0f);
    }

    if (Utility::EqualsIgnoreCase(object->GetName(), "Player")){
        HitEffect();
        pGameEventNotifier_->Notify("PlayerHit", statusCurrent_.getAttack());
        statusCurrent_.setHp(0);
        pCollider_->Disable();
    }

    if (Utility::EqualsIgnoreCase(object->GetName(), "Castle")){
        HitEffect();
        statusCurrent_.setHp(0);
        pCollider_->Disable();
    }

    if (statusCurrent_.getHp() <= 0)
    {
        pGameEventNotifier_->Notify("EnemyDeadForXP", statusCurrent_.getExperiencePoints());
        pCollider_->Disable();
        isDead_ = true;
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

    deltaTime_ = DeltaTimeManager::GetInstance()->GetDeltaTime(1);
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
