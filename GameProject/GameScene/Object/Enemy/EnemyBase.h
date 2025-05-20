#pragma once

#include "../Object.h"
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>

class EnemyBase : public Object
{
public:
    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }
    virtual void OnCollisionTrigger(const Collision::Collider* _other);

protected:
    void HitEffect();

protected:
    Object* pTarget_ = nullptr;
    GameEventNotifier* pGameEventNotifier_ = nullptr;

};
