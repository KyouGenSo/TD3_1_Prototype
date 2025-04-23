#pragma once
#include <GameScene/Object/Bullets/BulletBase.h>

#include <Timer/Timer.h>

#include "EmitterManager.h"

class RocketBullet : public BulletBase
{
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;

    void OnCollisionTrigger(const Collision::Collider* _other) override;

private:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;

    Vector3 forward_ = {};


    Vector4 color_ = { 1,1,1,1 };

    Timer timer;

    std::unique_ptr<Collision::Collider> explosion_;
    bool createExpl_ = false;
    Vector3 explosionPos_ = {};
    bool explode_ = false;
};

