#pragma once
#include "GameScene/Object/Bullets/BulletBase.h"

class MachineGunBullet : public BulletBase
{
    class Bullet : public BulletBase{
    public:
        void Initialize() override;
        void Update() override;
        void Draw() override;
        void OnCollisionTrigger(const Collision::Collider* _collider) override;

        void SetSpeed(float speed) {
            speed_ = speed;
        }
    protected:
        void InitializeNormal() override;
        void InitializeChain() override;
        void UpdateNormal() override;
        void UpdateChain() override;
    };

    std::vector<std::unique_ptr<Bullet>> bullets_ = {};
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void OnCollisionTrigger(const Collision::Collider* _collider) override;
    bool IsDeadAll() override;
protected:
    void InitializeNormal() override;
    void InitializeChain() override;
    void UpdateNormal() override;
    void UpdateChain() override;
};

