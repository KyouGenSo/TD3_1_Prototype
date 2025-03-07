#pragma once
#include "Bullet.h"

class RocketBullet : public Bullet{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Fire() override;

    void OnCollisionTrigger(const Object* _other) override;

private:
    void AttackNormal() override;
    void AttackChain() override;

    Vector3 forward_ = {};
};

