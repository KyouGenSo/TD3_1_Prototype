#pragma once
#include "GameScene/Object/Bullets/BulletBase.h"

class MachineGunBullet : public BulletBase{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void OnCollisionTrigger(const Collider* _collider) override;

protected:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;
};

