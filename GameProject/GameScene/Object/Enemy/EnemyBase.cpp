#include "EnemyBase.h"
#include <Type/ColliderType.h>

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
    }
}

void EnemyBase::HitEffect()
{
    if (emitter_){
        emitter_->SetEmitterPosition("hit", transform_.translate);
        emitter_->CreateTemporaryEmitterFrom("hit", "hit_tmp", 1.f);
    }
}
