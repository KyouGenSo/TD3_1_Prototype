#pragma once
#include <GameScene/Object/Bullets/BulletBase.h>

class RocketBullet : public BulletBase{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Fire() override;

    void OnCollisionTrigger(const Collider* _other) override;

private:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;

    Vector3 forward_ = {};

    std::unique_ptr<Collider> explosion_;
};

