#pragma once

#include "../Object.h"
#include <GameUI/HPBar/HPBar.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>
#include <Vector2.h>

class EnemyBase : public Object
{
public:
    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }
    virtual void OnCollisionTrigger(const Collision::Collider* _other);
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw2d();

protected:
    void HitEffect();
    void UpdateHPBarPosition();

protected:
    Object* pTarget_ = nullptr;
    std::unique_ptr<HPBar> pHPBar_ = nullptr;
    GameEventNotifier* pGameEventNotifier_ = nullptr;
    static const Vector2 hpBarSize_;

};
