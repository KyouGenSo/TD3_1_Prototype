#pragma once

#include <GameScene/Object/Bullets/BulletBase.h>
#include <array>

#include "Collision/Collider.h"

class AssaultBullet : public BulletBase
{
private:
    class Bullet : public BulletBase
    {
    public:
        void Initialize() override;
        void Update() override;
        void Draw() override;
        void OnCollisionTrigger(const Collision::Collider* _other) override;
        void SetSpeed(float _speed) { speed_ = _speed; }

    private:
        // BulletBase を介して継承されました
        void InitializeNormal() override;
        void InitializeChain() override;
        void UpdateNormal() override;
        void UpdateChain() override;
    };

public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void OnCollisionTrigger(const Collision::Collider* _other) override {};
    bool IsDeadAll() override;

private:
    std::unique_ptr<Bullet> bullet_;
    std::array<std::unique_ptr<Bullet>, 8> bullets_ = {};

private:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;
};
