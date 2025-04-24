#include "EnemyBase.h"

void EnemyBase::HitEffect()
{
    if (emitter_){
        emitter_->SetEmitterPosition("hit", transform_.translate);
        emitter_->CreateTemporaryEmitterFrom("hit", "hit_tmp", 1.f);
    }
}
