#pragma once

#include "../Object.h"
#include <GameUI/HPBar/HPBar.h>
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>
#include <Vector2.h>

class EnemyBase : public Object
{
public:
    enum Type
    {
        Normal,
        Fly,
        Bounce
    };

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }
    virtual void OnCollision(const Collision::Collider* _other) override;
    virtual void OnCollisionTrigger(const Collision::Collider* _other);
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Finalize();
    virtual void Draw2d();
    void SetTranslate(Vector3 translate) { transform_.translate = translate; }
    void SetIsAppearing(bool isAppearing) { isAppearing_ = isAppearing; }
    void SetAppearCounter(float appearCounter) { appearCounter_ = appearCounter; }

protected:
    void HitEffect();
    void UpdateHPBarPosition();

protected:
    Object* pTarget_ = nullptr;
    std::unique_ptr<HPBar> pHPBar_ = nullptr;
    GameEventNotifier* pGameEventNotifier_ = nullptr;
    Vector2 hpBarSize_ = {80.0f, 5.0f};;
    bool isAppearing_ = false;
    float appearCounter_ = 0.0f;
    Vector3 hpBarOffset_ {0,0, 0};
};
